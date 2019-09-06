---
title: 当前赛季比赛列表
date: 2019-09-05
---

<div id="contest-list"></div>

<script>
const base_url = 'https://icpc.baylor.edu/cm5-contest-rest/rest/contest/public/';
const finder_base_url = 'https://icpc.baylor.edu/regionals/finder/';

const regions = [
    'AR-Nannjing-2019',
    'Shanghai-2019',
    'Hong-Kong-City-2019',
    'Asia-Nanchang-2019',
    'Yinchuan-2019',
    'Shenyang-2019',
    'Xuzhou-2019',
    'Asia-EC-League-Final-2019',
];

var contests = [];

function safeDate(d) {
    if(d === undefined) {
        return 'to be determined';
    }
    return d;
}

function generateTable() {
    if(contests.length != regions.length) {
        return;
    }
    contests.sort((a, b) => {
        return a.startDate == b.startDate ? 0 :
            a.startDate === undefined || a.startDate > b.startDate ? 1 : -1;
    });
    let table = '<table><tr><th>Name</th><th>Start Date</th><th>End Date</th></tr>';
    for(let i in contests) {
        let contest = contests[i];
        let line = '<tr>';
        line += '<td><a href="' + finder_base_url + contest.url_name + '">' + contest.name + '<a/></td>';
        line += '<td>' + safeDate(contest.startDate) + '</td>';
        line += '<td>' + safeDate(contest.endDate) + '</td>';
        line += '</tr>';
        table += line;
    }
    table += '</table>';
    $('#contest-list').html(table);
}

async function getContest(name) {
    $.get(base_url + name, (data) => {
        data.url_name = name;
        contests.push(data);
        generateTable();
    });
}

regions.map(getContest);
</script>

