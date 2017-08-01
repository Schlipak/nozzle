#ifndef PANEL_HH
#define PANEL_HH

#include <QWidget>

namespace Ui {
  class Panel;
}

class Panel : public QWidget
{
  Q_OBJECT

public:
  explicit Panel(QWidget *parent = 0);
  ~Panel();

private:
  Ui::Panel *ui;
};

#endif // PANEL_HH
