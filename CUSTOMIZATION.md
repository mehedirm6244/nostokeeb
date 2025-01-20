## Customization

### Theming

Nostokeeb will by default follow your GTK3 theme. As it uses GTK3, you can theme it just like any other GTK3 app using ```css```. Here's an example `gtk.css`
```
.nk_keyboard * {
  all: unset; /* Remove all styles set by the GTK theme */
}
.nk_keyboard {
  background-color: #d0d3d9;
  color: #222;
}
.nk_keyboard button {
  padding: 8px;
  min-width: 24px;
  min-height: 24px;
  color: #333;
  font-weight: 500;
  background-color: #fff;
  border-radius: 4px;
  box-shadow: 0 3px 2px -3px rgba(0, 0, 0, 0.2);
}
.nk_keyboard button:hover {
  background-color: #eee;
}
.nk_keyboard button:active {
  background-color: #dedede;
}
.nk_keyboard button:checked {
  background-color: #6b8cf9;
  color: #eee;
}
.nk_keyboard .button-wide {
  padding: 8px 12px;
}
.nk_keyboard .control-box button {
  background-color: transparent;
  box-shadow: none;
  padding: 6px;
  min-width: 20px;
  min-height: 20px;
  border-radius: 0px;
}
.nk_keyboard .control-box button:hover {
  background-color: #eee;
}
```
