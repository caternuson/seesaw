#include "qpcpp.h"
#include "qp_extras.h"

#include "hsm_id.h"
#include "Foo.h"
#include "event.h"
#include "SeesawConfig.h"
#include "bsp_gpio.h"
#include "bsp_neopix.h"

Q_DEFINE_THIS_FILE

using namespace FW;

Foo::Foo() :
    QActive((QStateHandler)&Foo::InitialPseudoState),
    m_id(AO_FOO), m_name("Foo") {}

QState Foo::InitialPseudoState(Foo * const me, QEvt const * const e) {
    (void)e;
    me->subscribe(FOO_START_REQ);
    me->subscribe(FOO_STOP_REQ);
    me->subscribe(FOO_READ_REG_REQ);
    me->subscribe(FOO_WRITE_REG_REQ);

    return Q_TRAN(&Foo::Root);
}

QState Foo::Root(Foo * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            LOG_EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            LOG_EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status = Q_TRAN(&Foo::Stopped);
            break;
        }
        case FOO_STOP_REQ: {
            LOG_EVENT(e);
            status = Q_TRAN(&Foo::Stopped);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm::top);
            break;
        }
    }
    return status;
}

QState Foo::Stopped(Foo * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            LOG_EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            LOG_EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case FOO_STOP_REQ: {
            LOG_EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case FOO_START_REQ: {
            LOG_EVENT(e);
            status = Q_TRAN(&Foo::Started);
            break;
        }
        default: {
            status = Q_SUPER(&Foo::Root);
            break;
        }
    }
    return status;
}

QState Foo::Started(Foo * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            LOG_EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            LOG_EVENT(e);
            status = Q_HANDLED();
            break;
        }
        case FOO_READ_REG_REQ: {
            LOG_EVENT(e);
            FooReadRegReq const &req = static_cast<FooReadRegReq const &>(*e);
            Fifo *dest = req.getDest();
            uint8_t ret[] = {23, 42};
            ret[0] = me->m_data;
            dest->Write(ret, 2);
            Evt *evt = new DelegateDataReady(req.getRequesterId());
            QF::PUBLISH(evt, me);
            status = Q_HANDLED();
            break;
        }
        case FOO_WRITE_REG_REQ: {
            LOG_EVENT(e);
            FooWriteRegReq const &req = static_cast<FooWriteRegReq const &>(*e);
            me->m_data = req.getValue();
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&Foo::Root);
            break;
        }
    }
    return status;
}