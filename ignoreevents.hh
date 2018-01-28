#ifndef IGNOREEVENTS_H
#define IGNOREEVENTS_H

#include <QObject>

class IgnoreEvents : public QObject
{
public:
    explicit IgnoreEvents() = default;
    IgnoreEvents( const IgnoreEvents& ) = delete;
    IgnoreEvents operator=( const IgnoreEvents& ) = delete;
    ~IgnoreEvents() = default;

    bool eventFilter( QObject* o, QEvent* e );
};

#endif // IGNOREEVENTS_H
