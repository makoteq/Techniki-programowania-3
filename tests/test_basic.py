from __future__ import annotations

import scikit_build_example as m

# span, path
m.visualizeSignal(100, "./test-audio.wav")

# span, path
m.differenceSignal(100, "./test-audio.wav")

# mode, frequency
m.generateSignal(3, 5)

# first array for real, second for imaginary
m.Discrete_Fourier_transform([1, 2, 0, -1], [0, -1, -1, 2])
