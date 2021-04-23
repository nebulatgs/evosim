function toggleFade(checkboxElem) {
    Module._setFade(checkboxElem.checked);
}

function toggleReverseA(checkboxElem) {
    Module._reverseA(checkboxElem.checked);
}

function changeSpeed(slider) {
    if (!document.getElementById("pauseButton").checked)
        Module._setSpeed(slider.value);
}

function togglePause(pause) {
    Module._changePause(pause.checked, document.getElementById("speed").value);
    if (pause.checked)
        toPlay();
    else
        toPause();
}

function nextFrame() {
    Module._nextFrame(document.getElementById("speed").value);
}

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

const element = document.getElementById('resetSpin')

function reset() {
    element.classList.remove('resetAnim'); // reset animation
    void(element.clientWidth); // trigger reflow
    document.getElementById('resetSpin').classList.add('resetAnim'); // start animation
    Module._restart();
}

var browser = bowser.getParser(window.navigator.userAgent);
var browserName = browser.getBrowser().name;
if (browserName == "Firefox") {
    let chrome = document.getElementById("chrome");
    chrome.innerHTML = "";
    chrome.remove();
} else {
    let firefox = document.getElementById("firefox");
    firefox.innerHTML = "";
    firefox.remove();
}