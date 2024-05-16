#include <pybind11/pybind11.h>
#include <iostream>
#include <matplot/matplot.h>
#include <cmath>
#include "AudioFile.h"
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <complex>
#include <string>
#include <vector>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)
#define E 2.718281828459045

std::vector <std::string>
Discrete_Fourier_transform(std::vector<double> signals_array_real, std::vector<double> signals_array_imaginary) {
    using namespace std::complex_literals;
    std::vector <std::complex<double>> signals_array;
    for (int i = 0; i < signals_array_real.size(); i++) {
        signals_array.push_back({signals_array_real.at(i), signals_array_imaginary.at(i)});
    }
    std::vector <std::complex<double>> signals_array_afterDFS;
    std::complex<double> transform_value = 0;
    std::complex<double> transform_value_prev = 0;
    std::complex<double> summ = 0;
    for (int k = 0; k < signals_array.size(); k++) {
        transform_value = 0;
        transform_value_prev = 0;
        summ = 0;
        for (int n = 0; n < signals_array.size(); n++) {
            std::complex<double> WN = ((-6.28318530718i * (double) n * ((double) k) / (double) signals_array.size()));
            std::complex<double> Euler_pow = std::pow(E, WN);
            transform_value_prev = signals_array.at(n) * Euler_pow;

            summ = transform_value_prev + transform_value;
            transform_value = summ;
        }
        signals_array_afterDFS.push_back(transform_value);
    }
    std::vector <std::string> signals_array_x_after;
    std::vector <std::string> signals_array_y_after;
    std::vector <std::string> signals_output;
    for (int i = 0; i < signals_array.size(); i++) {
        signals_array_x_after.push_back(std::to_string((int) round(real(signals_array_afterDFS.at(i)))));
        signals_array_y_after.push_back(std::to_string((int) round(imag(signals_array_afterDFS.at(i)))));
        signals_output.push_back(signals_array_x_after.at(i) + ' ' + signals_array_y_after.at(i) + 'i');
        std::cout << signals_output.at(i) << std::endl;
    }
    return signals_output;
}


int visualizeSignal(int span, const char *path) {

    AudioFile<double> audioFile;
    audioFile.load(path);

    std::vector<double> x;
    std::vector<double> y;

    int channel = 0;

    for (int i = 0; i < span; i++) {
        x.push_back(i);
        y.push_back(audioFile.samples[channel][i] * 4);
    }

    matplot::plot(x, y, "-o");

    matplot::show();
}

int differenceSignal(int span, std::string path) {

    AudioFile<double> audioFile;
    audioFile.load(path);

    std::vector<double> x;
    std::vector<double> y;

    int channel = 0;

    for (int i = 0; i < span; i++) {
        x.push_back(i);
        //  double diff = audioFile.samples[channel][i+1]-audioFile.samples[channel][i];
        y.push_back(audioFile.samples[channel][i + 1] - audioFile.samples[channel][i]);
    }
    matplot::ylim({-1, +1});
    matplot::plot(x, y, "-o");
    matplot::show();
    return 0;
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
            for (int i = 0; i < SPAN; i++) {
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
            for (int i = 0; i < interval + 1; i++) {
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

m.def("Discrete_Fourier_transform", &Discrete_Fourier_transform, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");
m.def("visualizeSignal", &visualizeSignal, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");
m.def("generateSignal", &generateSignal, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");
m.def("differenceSignal", &differenceSignal, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

#ifdef VERSION_INFO
m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
m.attr("__version__") = "dev";
#endif
}
