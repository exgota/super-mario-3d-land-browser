#include <clean/LayoutDrawerPrefix.h>

extern "C" void fn_0021503C(nw::lyt::Drawer* drawer, const unsigned* colors,
                           unsigned alpha) {
    const float channelScale = 1.0f / 255.0f;
    float opacity = static_cast<float>(alpha) * channelScale;
    nw::lyt::ObservedColor4f converted[4];
    bool allWhite = true;
    for (int vertex = 0; vertex < 4; ++vertex) {
        if (colors[vertex] != 0xffffffffU) {
            const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&colors[vertex]);
            for (int channel = 0; channel < 4; ++channel)
                converted[vertex].component[channel] = static_cast<float>(bytes[channel]) * channelScale;
            for (int previous = 0; allWhite && previous < vertex; ++previous) {
                for (int channel = 0; channel < 3; ++channel)
                    converted[previous].component[channel] = 1.0f;
                converted[previous].component[3] = opacity;
            }
            allWhite = false;
            converted[vertex].component[3] *= opacity;
        } else if (!allWhite) {
            for (int channel = 0; channel < 3; ++channel)
                converted[vertex].component[channel] = 1.0f;
            converted[vertex].component[3] = opacity;
        }
    }
    if (allWhite) {
        drawer->colorSelector = opacity;
        return;
    }
    drawer->colorSelector = static_cast<float>(drawer->colorCount);
    for (int vertex = 0; vertex < 4; ++vertex)
        for (int channel = 0; channel < 4; ++channel)
            drawer->colorBuffer[drawer->colorCount + vertex].component[channel] = converted[vertex].component[channel];
    drawer->colorCount += 4;
}
