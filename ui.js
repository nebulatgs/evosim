function toggleFade(checkboxElem) {
    _setFade(checkboxElem.checked);
}

function changeSpeed(slider) {
    if (!document.getElementById("pauseButton").checked)
        _setSpeed(slider.value);
}

function togglePause(pause) {
    _changePause(pause.checked, document.getElementById("speed").value);
    if (pause.checked)
        toPlay();
    else
        toPause();
}

function nextFrame() {
    _nextFrame(document.getElementById("speed").value);
}

// var svg = document.getElementById("playpause");
// var s = Snap(svg);

// var play = Snap.select('#play');
// var pause = Snap.select('#pause');

// var playPoints = play.node.getAttribute('d');
// var pausePoints = pause.node.getAttribute('d');
let playAnims = document.getElementsByClassName("toPlay");


let pauseAnims = document.getElementsByClassName("toPause");


var toPause = function() {
    Array.prototype.filter.call(pauseAnims, function(pauseAnim) {
        return pauseAnim.beginElement();
    });
}

var toPlay = function() {

    Array.prototype.filter.call(playAnims, function(playAnim) {
        return playAnim.beginElement();
    });
}