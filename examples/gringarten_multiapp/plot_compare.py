#!/usr/bin/env python3
# * This file is part of the MOOSE framework
# * https://www.mooseframework.org
# *
# * All rights reserved, see COPYRIGHT for full restrictions
# * https://github.com/idaholab/moose/blob/master/COPYRIGHT
# *
# * Licensed under LGPL 2.1, please see LICENSE for details
# * https://www.gnu.org/licenses/lgpl-2.1.html
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math

from mpmath import *


# -------------------------------------------------------------------------------
df = pd.read_csv("mrect1_out_fracture_app0.csv")
print("fracture dataframe description: ", df.columns.values)
["time" "J_out" "P_in" "P_out" "TK_in" "TK_out" "kg_out" "kg_per_s"]
df_50 = pd.read_csv("mrect1_out_fracture_app0_d50.csv")
# analytic solution from Koenradd and Gringarten paper, 1975
timevector = np.logspace(6, 11, 50, base=10)

# analytic solution for single fracture Gringarten 1975 eqn A18
# from Beckers, Koenraad <Koenraad.Beckers@nrel.gov>

# terms for infinite and finite space fractures
Trock = 363 - 273.15
Tinj = 303 - 273.15
rhowater = 1000
cpwater = 4150
fracnumb = 1
fracwidth = 100
fracsep = 1000
krock = 2.83
fracheight = 10
rhorock = 2875
cprock = 825
mtot = 25 / 25 / 10
Q = mtot / rhowater / fracnumb / fracwidth
# terms needed for finite spaced fractures.  Gringarten 1975 eqn A19
xe = 20  # 1/2 spacing between fractures
z = fracheight


# nonDimensionalized Time eqn 10
td = (
    (rhowater * cpwater)
    * (rhowater * cpwater)
    / (4 * krock * rhorock * cprock)  # where does the 4 come from?  Its not in eqn 10
    * (Q / fracheight)
    * (Q / fracheight)
    * timevector
)
# Solution to eqn A18 -- infinite space between fractures
Twater = []
Twd_inf = []
for i in range(len(td)):
    # eqn A18 solving for nonDimensionalized temperature
    temp = 1 - math.erf(1 / (2 * math.sqrt(td[i])))
    Twd_inf.append(temp)
    # convert to Temperature
    Twater.append(Trock - temp * (Trock - Tinj))

# Solution to eqn A19 -- finite space between fractures set above by xe
# simplified inverse laplace transform
mp.dps = 15
top = rhowater * cpwater * Q * xe
bottom = 2 * krock * fracheight
zd = z / fracheight
Twd_tilde = lambda s: 1 / s * exp(-zd * sqrt(s) * tanh(top / bottom * sqrt(s)))
Twater_finite = []
for i in range(len(td)):
    temp = invertlaplace(Twd_tilde, td[i], method="talbot")
    # convert to Temperature
    Twater_finite.append(Trock - temp * (Trock - Tinj))

# -------------------------------------------------------------------------------
fig1 = plt.figure()
plt.plot(
    timevector / 3600 / 24 / 365,
    Twater,
    "b",
    linestyle="-",
    linewidth=1,
    marker="o",
    alpha=0.5,
    fillstyle="none",
    label="Gringarten xe=inf",
)
plt.plot(
    timevector / 3600 / 24 / 365,
    Twater_finite,
    "k",
    linestyle="-",
    linewidth=1,
    marker="^",
    alpha=0.5,
    fillstyle="none",
    label="Gringarten xe=20m",
)
plt.plot(
    df_50["time"] / 3600 / 24 / 365,
    df_50["TK_out"] - 273.15,
    "r",
    linestyle="--",
    linewidth=2,
    label="Falcon xe=50m",
)
plt.plot(
    df["time"] / 3600 / 24 / 365,
    df["TK_out"] - 273.15,
    "g",
    linestyle="-.",
    linewidth=2,
    label="Falcon xe=20m",
)
plt.ylim([30, 100])
plt.xlim([0, 10])
plt.ylabel("T (degC)")
plt.xlabel("Time (year)")
plt.grid()
plt.legend()
plt.title("production temperature")

plt.savefig("compareGringarten.pdf")

plt.show()
