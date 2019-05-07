---
title: 北航春训 DOMjudge 配置坑点
date: 2019-04-28
category: 随记
tags: DOMjudge
portal_links:
    DOMjudge: https://www.domjudge.org/
---

> DOMjudge 版本：6.0.3

<!-- more -->

总体上来讲，按照官方的<!--
--> [admin manual](https://www.domjudge.org/docs/admin-manual.html) <!--
-->一步一步来不会出错的。<!--
-->出现问题时，可以先查手册，然后去查 [maillist](https://www.domjudge.org/pipermail/domjudge-devel/)。

推荐使用最新的（18.10） Ubuntu Server。


## 测试用题目包

[ChieloNewctle/domjudge-contest-sample](https://github.com/ChieloNewctle/domjudge-contest-sample)


## domserver

### 部署

没什么好说的，按官方说明的一步一步走即可。

新版本中，在 Jury interface 提供了 Config Checker，<!--
-->可以检查很多配置是否合理，包括一部分的 Finetune 参数。

### 数据导入导出

源码在 `domserver/lib/lib.impexp.php` 中。

#### 导入 `teams.tsv`

先导入 `teams.tsv` 再导入 `accounts.tsv`

##### `teams.tsv` 格式
```
File_Version    1
唯一的队伍ID\t	唯一的外部ID\t	team_category的ID\t	队名\t	校名\t	校名（短）\t	国家\t	唯一的校名外部ID
```

##### `teams.tsv` 样例
```
File_Version    1
1391	springA1391	3	name	Beihang University	BUAA	CHN	BUAA
```

#### 导入 `accounts.tsv`

`accounts.tsv` 导入的时候也会自动去挂队伍ID，<!--
-->但方式是跳过 `username` 前面的非数字字符、再跳过 `0` 以后，<!--
-->以剩下的数字字符作为对应的队伍ID（剩下的不是数字就不挂队伍了）。<!--
-->对应的源码：
```
$teamid = preg_replace('/^[^0-9]*0*([0-9]+)$/', '\1', $line[2]);
```

##### `accounts.tsv` 格式
```
File_Version    1
角色（team, admin等）\t	名字\t	username\t	password
```

##### `accounts.tsv` 样例
```
File_Version    1
team	Chielo	chielo	P@ssw0rd
```

## judgehost

### 部署

在 [admin manual](https://www.domjudge.org/docs/admin-manual.html) 中的 Quick installation 介绍了大致的流程

> On the judgehosts:
> - `useradd -d /nonexistent -U -M -s /bin/false domjudge-run`
> - Add to `/etc/sudoers.d/` or append to `/etc/sudoers` the sudoers configuration as in etc/sudoers-domjudge. （推荐前者）
> - Set up cgroup support: enable kernel parameters in `/etc/default/grub` and reboot, then use `misc/create_cgroups` to create cgroups for DOMjudge.
> - Put the right credentials in the file `etc/restapi.secret` on all judgehosts (copied from the domserver). （正常来讲，build 好后 install 不会需要这一步）
> - Start the judge daemon: `bin/judgedaemon` （推荐用 screen 单独跑）

但这只是简单的介绍，具体的还是需要根据 admin manual 后文一步一步来。

#### cgroups

"enable kernel parameters in `/etc/default/grub` and reboot" 需要参考节 "Linux Control Groups"<!--
--> 修改后运行 `update-grub`，否则只修改 `/etc/default/grub` 重启也不会起效。

运行 judgehost 前记得运行 `create_cgroups`，建立 cgroups。

#### 单机多 judgehosts

比如我想用 `judgehost/bin/judgedaemon -n 2` 在主机上运行第二个评测机，<!--
-->则需要运行一次 `useradd -d /nonexistent -U -M -s /bin/false domjudge-run-2` 建立用户。

### 运行机制

在搭建好的 domjudge 的 jury interface 中，会有 Languages 和 Executables 的配置。<!--
-->这些决定了哪些语言可以交，长什么样，怎么测。

#### Time factor

实际是 **Time limitation factor**，即原题面的时限乘这个因子作为该语言的时限。<!--
-->比如可以给 Java 配置两倍的 Time factor，这样 1 秒钟时限的题目，使用 Java 提交的话时限会放宽到 2 秒钟。

#### Compile script

这个脚本负责编译参赛者提交的程序。可以是一个 shell 脚本。<!--
-->第一个参数表示要求最终生成的，有 executable 权限的文件路径。

比如 C++，就可以直接把二进制文件生成到目标路径上；<!--
-->Java，就需要生成一个有 executable 权限的脚本，调用 `java` 去运行。

**编译时是在 `judgedaemon` 运行的环境中进行的，以 `domjudge-run` 用户运行的。**

#### chroot

Compile script 编译后，运行编译得到的文件时，<!--
-->**是将之前建立好的 chroot 环境中的若干目录，挂载到<!--
--> `judgehost/judgings/主机名/endpoint-default/提交id/` 相应的目录中运行的。**

因此在更新主机的环境后，chroot 相应的环境也应当进行更新，同步版本。

若有多台评测机时，注意同步运行环境。


## kotlin

推荐使用 [JetBrains 官方 Github 中的 Release](https://github.com/JetBrains/kotlin/releases)。

在 judgehost 的主机中，配置到让各个用户都可以调用 `kotlinc`，便可以编译 Kotlin 程序。

在 judgehost 的 chroot 环境中，<!--
-->将 `lib` 的内容解压到 `chroot` 中的 `lib`，<!--
-->`bin` 的内容解压到 `chroot` 中的 `bin`，<!--
-->将 `chroot/bin` 中的 `kotlin` 等链接到 `chroot/usr/bin` 中，<!--
-->这样编译得到的 Kotlin 程序就可以在 judgehost 的 chroot 环境中运行了。

