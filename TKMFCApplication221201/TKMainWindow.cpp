#include "TKMainWindow.h"

void TKMainWindow::PostNcDestroy() {
	delete this;
}