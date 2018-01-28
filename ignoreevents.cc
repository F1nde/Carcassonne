#include "ignoreevents.hh"

#include <QEvent>


bool IgnoreEvents::eventFilter(QObject *o, QEvent *e)
{
    if( e->type() == QEvent::KeyPress or
            e->type() == QEvent::MouseButtonPress )
    {
        return true;
    } else {
        return o->parent()->eventFilter( o, e );
    }
}
