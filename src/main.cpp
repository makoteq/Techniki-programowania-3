#include <pybind11/pybind11.h>
#include <matplot/matplot.h>
#include <cmath>
#include "AudioFile.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int visualizeSignal(int span, const char *path) {

    AudioFile<double> audioFile;
    audioFile.load(path);

    std::vector<double> x;
    std::vector<double> y;

    int channel = 0;

    for (int i = 0; i < span; i++) {
        x.push_back(i);
        y.push_back(audioFile.samples[channel][i]);
    }

    matplot::plot(x, y, "-o");

    matplot::show();
}

int generateSignal(int mode, int interval) {
    std::vector<double> x;
    std::vector<double> y;
    int index = 0;
    bool state = true;
    int SPAN(10 * interval);
    switch (mode) {
        case 0 :
            for (float i = 0; i < SPAN + 1; i++) {
                x.push_back(i);
                y.push_back(sin((i / (SPAN / 2 / interval)) * matplot::pi));
            }
            matplot::ylim({-1.5, +1.5});
            matplot::plot(x, y, "-o");
            break;
        case 1 :
            for (float i = 0; i < SPAN + 1; i++) {
                x.push_back(i);
                y.push_back(cos((i / (SPAN / 2 / interval)) * matplot::pi));
            }
            matplot::ylim({-1.5, +1.5});
            matplot::plot(x, y, "-o");
            break;
        case 2:
            for (int i = 0; i < SPAN + 1; i++) {
                if (index == (SPAN / (interval * 2))) {
                    state = !state;
                    index = 0;
                }
                x.push_back(i);
                y.push_back(state);
                index++;
            }
            matplot::stairs(x, y, "-o");
            matplot::ylim({-1, +2});
            break;
        case 3:
            state = false;
            x.push_back(0);
            y.push_back(state);
            state = true;
            for (int i = 0; i < interval; i++) {
                if (i) {
                    x.push_back(i);
                    y.push_back(state);
                    state = false;
                    x.push_back(i);
                    y.push_back(state);
                    state = true;
                }

            }
            matplot::plot(x, y, "-o");
            matplot::ylim({-1, +2});
            break;

    }

    matplot::xlabel("number of samples in 1 second");
    matplot::ylabel("amplitude");
    matplot::show();
    return 0;
}

namespace py = pybind11;

PYBIND11_MODULE(_core, m
) {
m.

doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: scikit_build_example

        .. autosummary::
           :toctree: _generate

           visualizeSignal
           generateSignal
    )pbdoc";

m.def("visualizeSignal", &visualizeSignal, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");
m.def("generateSignal", &generateSignal, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

#ifdef VERSION_INFO
m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
m.attr("__version__") = "dev";
#endif
}
