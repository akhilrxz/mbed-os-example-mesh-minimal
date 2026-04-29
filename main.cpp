#include "mbed.h"
#include "efr32fg28b322f1024im48.h"
#include "fg28_rtt.h"

namespace {
constexpr uint32_t LED_PIN = 5U;
constexpr uint32_t LED_MASK = (1UL << LED_PIN);
constexpr uint32_t MODE_SHIFT = (LED_PIN * 4U);
constexpr uint32_t MODE_MASK = (0xFUL << MODE_SHIFT);
constexpr uint32_t MODE_PUSHPULL = (GPIO_P_MODEL_MODE0_PUSHPULL << MODE_SHIFT);
}

int main()
{
    RTT_STAGE("main() entered - BOOT SUCCESS");
    RTT_PRINTF("[main] LED=PC%u  HFXO=39MHz  NS-mode\n", (unsigned)LED_PIN);

    /*
     * LED: PC05 on PHYWAVE_FG28 Rev1P0.
     * Access GPIO through NS alias only.
     */
    GPIO_NS->P[gpioPortC].MODEL =
        (GPIO_NS->P[gpioPortC].MODEL & ~MODE_MASK) | MODE_PUSHPULL;
    GPIO_NS->P[gpioPortC].DOUT &= ~LED_MASK;
    RTT_STAGE("main: GPIO init done, starting blink loop");

    uint32_t blink_count = 0;
    while (true) {
        GPIO_NS->P[gpioPortC].DOUT ^= LED_MASK;
        ++blink_count;
        if ((blink_count & 0xFU) == 0U) {
            RTT_PRINTF("[blink] count=%u\n", (unsigned)blink_count);
        }
        ThisThread::sleep_for(500ms);
    }
}
