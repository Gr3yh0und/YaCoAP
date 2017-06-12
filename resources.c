#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "coap.h"

#include "measurement.h"

#ifdef OPENTHREAD_ACTIVE
#include "openthread/platform/alarm.h"
#include "openthread/platform/logging.h"
#endif

#if OPENTHREAD_ENABLE_COAPS_CLI == 0
#define otPlatLog(...)
#endif

#define SERVICE "CoAPS"

static char light = '0';
const uint16_t rsplen = 128;
static char rsp[128] = "";

void resource_setup(const coap_resource_t *resources)
{
    coap_make_link_format(resources, rsp, rsplen);
#if YACOAP_DEBUG
#ifdef OPENTHREAD_ACTIVE
    otPlatLog(kLogLevelDebg, kLogRegionPlatform, "%d(%s): resources = %s", otPlatAlarmGetNow(), SERVICE, rsp);
#else
    printf("%s resources: %s\n", SERVICE, rsp);
#endif
#endif
}

static const coap_resource_path_t path_well_known_core = {2, {".well-known", "core"}};
static int handle_get_well_known_core(const coap_resource_t *resource,
                                      const coap_packet_t *inpkt,
                                      coap_packet_t *pkt)
{
#if YACOAP_DEBUG
    printf("%s: handle_get: well_known_core\n", SERVICE);
#endif
    return coap_make_response(inpkt->hdr.id, &inpkt->tok,
                              COAP_TYPE_ACK, COAP_RSPCODE_CONTENT,
                              resource->content_type,
                              (const uint8_t *)rsp, strlen(rsp),
                              pkt);
}

static const coap_resource_path_t path_light = {1, {"light"}};
static int handle_get_light(const coap_resource_t *resource,
                            const coap_packet_t *inpkt,
                            coap_packet_t *pkt)
{
#if YACOAP_DEBUG
    printf("%s: handle_get: light\n", SERVICE);
#endif
    return coap_make_response(inpkt->hdr.id, &inpkt->tok,
                              COAP_TYPE_ACK, COAP_RSPCODE_CONTENT,
                              resource->content_type,
                              (const uint8_t *)&light, 1,
                              pkt);
}

static int handle_put_light(const coap_resource_t *resource,
                            const coap_packet_t *inpkt,
                            coap_packet_t *pkt)
{
#if YACOAP_DEBUG
    printf("%s: handle_put_light\n", SERVICE);
#endif
    if (inpkt->payload.len == 0) {
        return coap_make_response(inpkt->hdr.id, &inpkt->tok,
                                  COAP_TYPE_ACK, COAP_RSPCODE_BAD_REQUEST,
                                  NULL, NULL, 0,
                                  pkt);
    }
    if (inpkt->payload.p[0] == '1') {
        light = '1';
#if YACOAP_LEDS_ENABLED
        LED0_ON;
#endif
#if YACOAP_DEBUG
        printf("%s: Light ON\n", SERVICE);
#endif
    }
    else {
        light = '0';
#if YACOAP_LEDS_ENABLED
        LED0_OFF;
#endif
#if YACOAP_DEBUG
        printf("%s: Light OFF\n", SERVICE);
#endif
    }
    return coap_make_response(inpkt->hdr.id, &inpkt->tok,
                              COAP_TYPE_ACK, COAP_RSPCODE_CHANGED,
                              resource->content_type,
                              (const uint8_t *)&light, 1,
                              pkt);
}

coap_resource_t resources[] =
{
    {COAP_RDY, COAP_METHOD_GET, COAP_TYPE_ACK,
        handle_get_well_known_core, &path_well_known_core,
        COAP_SET_CONTENTTYPE(COAP_CONTENTTYPE_APP_LINKFORMAT)},
    {COAP_RDY, COAP_METHOD_GET, COAP_TYPE_ACK,
        handle_get_light, &path_light,
        COAP_SET_CONTENTTYPE(COAP_CONTENTTYPE_TXT_PLAIN)},
    {COAP_RDY, COAP_METHOD_PUT, COAP_TYPE_ACK,
        handle_put_light, &path_light,
        COAP_SET_CONTENTTYPE(COAP_CONTENTTYPE_NONE)},
    {(coap_state_t)0, (coap_method_t)0, (coap_msgtype_t)0,
        NULL, NULL,
        COAP_SET_CONTENTTYPE(COAP_CONTENTTYPE_NONE)}
};
