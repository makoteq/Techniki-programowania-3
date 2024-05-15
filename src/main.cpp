#include <pybind11/pybind11.h>
#include <matplot/matplot.h>
#include "AudioFile.h"
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


int add(int i, int j){

namespace plt = matplot;

    AudioFile<double> audioFile;
    audioFile.load ("./test-audio.wav");

    std::vector<double> x;
    std::vector<double> y;

    int channel = 0;

    for(int i = 0; i < 100; i++)
    {
        x.push_back(i);
        y.push_back(audioFile.samples[channel][i]);
    }

    plt::plot(x, y, "-o");

    plt::show();
}

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: scikit_build_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
