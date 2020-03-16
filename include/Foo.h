#ifndef AO_FOO_H
#define AO_FOO_H

#include "qpcpp.h"
#include "qp_extras.h"

#include "hsm_id.h"

using namespace QP;
using namespace FW;

class Foo : public QActive {
public:
    Foo();
    void Start(uint8_t prio) {
        QActive::start(prio, m_evtQueueStor, ARRAY_COUNT(m_evtQueueStor), NULL, 0);
    }

protected:
    static QState InitialPseudoState(Foo * const me, QEvt const * const e);
    static QState Root(Foo * const me, QEvt const * const e);
    static QState Stopped(Foo * const me, QEvt const * const e);
    static QState Started(Foo * const me, QEvt const * const e);

    enum {
        EVT_QUEUE_COUNT = 8,
    };
    QEvt const *m_evtQueueStor[EVT_QUEUE_COUNT];
    uint8_t m_id;
	uint16_t m_nextSequence;
	uint8_t m_speed;
    char const * m_name;

	uint8_t m_data;
};


#endif