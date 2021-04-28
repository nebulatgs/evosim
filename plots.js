var trace0 = {
    x: [0],
    y: [0],
    marker: {
        size: 5,
        color: '#47e5bc'
    },
    mode: 'lines+markers',
    type: 'scatter',
};

var trace1 = {
    x: [0],
    y: [15],
    marker: {
        size: 5,
        color: '#da627d'
    },
    mode: 'lines+markers',
    type: 'scatter'
};

var trace2 = {
    x: [0],
    y: [2],
    marker: {
        size: 5,
        color: '#b7c3f3'
    },
    mode: 'lines+markers',
    type: 'scatter',
};
// var data = [trace0];

var resLayout = {
    width: 200,
    height: 200,
    margin: {
        l: 0,
        r: 0,
        t: 0
    },
    legend: {
        orientation: "h"
    },
    xaxis: {
        showgrid: false,
        zeroline: false,
        showticklabels: false,
    },
    yaxis: {
        range: [0, 100],
        showgrid: true,
        zeroline: true,
        showticklabels: true,
        gridcolor: "#494368",
        zerolinecolor: "#7e77a7",
    },
    paper_bgcolor: "rgba(0,0,0,0)",
    plot_bgcolor: "#000",
    showlegend: false,
}

var size = 2.5;
var maxSize = 2.5;

var sizeLayout = {
    width: 200,
    height: 200,
    margin: {
        l: 0,
        r: 0,
        t: 0
    },
    legend: {
        orientation: "h"
    },
    xaxis: {
        showgrid: false,
        zeroline: false,
        showticklabels: false,
    },
    yaxis: {
        range: [2, 2.5],
        showgrid: true,
        zeroline: true,
        showticklabels: true,
        gridcolor: "#494368",
        zerolinecolor: "#7e77a7",
    },
    paper_bgcolor: "rgba(0,0,0,0)",
    plot_bgcolor: "#000",
    showlegend: false,
}

var count = 0;
var maxCount = 0;


var countLayout = {
    width: 200,
    height: 200,
    margin: {
        l: 0,
        r: 0,
        t: 0
    },
    legend: {
        orientation: "h"
    },
    xaxis: {
        showgrid: false,
        zeroline: false,
        showticklabels: false,
    },
    yaxis: {
        range: [0, maxCount],
        showgrid: true,
        zeroline: true,
        showticklabels: true,
        gridcolor: "#494368",
        zerolinecolor: "#7e77a7",
    },
    paper_bgcolor: "rgba(0,0,0,0)",
    plot_bgcolor: "#000",
    showlegend: false,
}

var config = {
    responsive: true,
    staticPlot: true,
}

// Plotly.newPlot('plot1', data, layout);

Plotly.newPlot('plot1', [trace0], countLayout, config);
Plotly.newPlot('plot2', [trace1], resLayout, config);
Plotly.newPlot('plot3', [trace2], sizeLayout, config);

// var iteration = -10;

function updatePlot() {
    if (document.getElementById("pauseButton").checked) { return; }
    // iteration++;
    // if (iteration % 10 != 0) {
    //     // window.requestAnimationFrame(updatePlot);
    //     return;
    // }
    // if (iteration > 0) {
    if (trace0.x.length > 100) {
        trace0.x.shift();
        trace0.y.shift();
    }
    trace0.x.push(trace0.x[trace0.x.length - 1] + 1);
    count = Module._getCount();
    if (count > maxCount) { maxCount = count; }
    trace0.y.push(count);
    countLayout.yaxis.range[1] = maxCount * 1.2;

    if (trace1.x.length > 100) {
        trace1.x.shift();
        trace1.y.shift();
    }
    trace1.x.push(trace1.x[trace1.x.length - 1] + 1);
    trace1.y.push(Module._getAvgResistance());

    if (trace2.x.length > 100) {
        trace2.x.shift();
        trace2.y.shift();
    }
    trace2.x.push(trace2.x[trace2.x.length - 1] + 1);
    size = Module._getAvgSize();
    if (size > maxSize) { maxSize = size; }
    trace2.y.push(size);
    sizeLayout.yaxis.range[1] = maxSize * 1.2;

    Plotly.update('plot1', [trace0], countLayout, [0]);
    Plotly.update('plot2', [trace1], resLayout, [0]);
    Plotly.update('plot3', [trace2], sizeLayout, [0]);

    // window.requestAnimationFrame(updatePlot);
    return;
    // } else {
    //     // window.requestAnimationFrame(updatePlot);
    //     return;
    // }
}
// window.requestAnimationFrame(updatePlot);
setInterval(updatePlot, 100);