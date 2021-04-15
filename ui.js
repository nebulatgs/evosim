function toggleFade(checkboxElem) {
    if (checkboxElem.checked) {
      _setFade(true);
    } else {
      _setFade(false);
    }
  }

function changeSpeed(slider) {
  _setSpeed(slider.value);
}