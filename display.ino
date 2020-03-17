/*
   ディスプレイ表示用
*/

void printDisplay(String msg) {
  display.clear();
  display.drawString(0, 0, msg);
  display.display();
}
