import os
import time
import argparse
import pandas as pd
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument(
    "--falcon_input",
    default="FORGE_NS_Ph3_coarse.i",
    help="input file of the native state model",
)
parser.add_argument(
    "--falcon_run", default="~/projects/falcon/falcon-opt", help="path to falcon"
)
parser.add_argument("--n_cores", type=int, default=2, help="cpu cores")
parser.add_argument("--max_iter", type=int, default=1, help="max iterations")
parser.add_argument("--nK", type=int, default=2, help="number of parameters")
parser.add_argument("--precision", type=float, default=1.0e-5, help="precision")
parser.add_argument("--alpha", type=float, default=0.01, help="line search step length")
parser.add_argument("--K_Perm", type=float, default=-16.0, help="initial permeability")
parser.add_argument(
    "--K_Cond", type=float, default=1.0, help="initial thermal conductivity"
)
parser.add_argument("--folder_ref", default="./Ref/", help="path to reference data")
parser.add_argument("--outf", default="./Results", help="output folder")
par = parser.parse_args()

outf = par.outf
data_f = outf + "/data"  # output data
try:
    os.makedirs(data_f)
except OSError:
    pass

falcon_input = par.falcon_input
falcon_run = par.falcon_run
n_cores = par.n_cores
max_iter = int(par.max_iter)
nK = par.nK
folder_ref = par.folder_ref  # reference data
folder_simul = "./"  # simulated data
num_file = 7  # number of simulation time steps


## Define a function to read data
def Read_data(folder, num_file):
    # data comes from GDR repository https://gdr.openei.org/submissions/1397
    well_16A = "FORGE_NS_Ph3_coarse_point_data_16A_"
    well_56 = "FORGE_NS_Ph3_coarse_point_data_56_32_"
    well_58 = "FORGE_NS_Ph3_coarse_point_data_58_32_"
    well_78 = "FORGE_NS_Ph3_coarse_point_data_78_32_"
    well_78B = "FORGE_NS_Ph3_coarse_point_data_78B_32_"

    (
        data_pressure_16A,
        data_temper_16A,
        data_dispi_16A,
        data_dispj_16A,
        data_dispk_16A,
    ) = ([], [], [], [], [])
    data_pressure_56, data_temper_56, data_dispi_56, data_dispj_56, data_dispk_56 = (
        [],
        [],
        [],
        [],
        [],
    )
    data_pressure_58, data_temper_58, data_dispi_58, data_dispj_58, data_dispk_58 = (
        [],
        [],
        [],
        [],
        [],
    )
    data_pressure_78, data_temper_78, data_dispi_78, data_dispj_78, data_dispk_78 = (
        [],
        [],
        [],
        [],
        [],
    )
    (
        data_pressure_78B,
        data_temper_78B,
        data_dispi_78B,
        data_dispj_78B,
        data_dispk_78B,
    ) = ([], [], [], [], [])

    for i in range(1, num_file + 1):
        string = str(i)
        str_16A = well_16A + string.zfill(4) + ".csv"
        str_56 = well_56 + string.zfill(4) + ".csv"
        str_58 = well_58 + string.zfill(4) + ".csv"
        str_78 = well_78 + string.zfill(4) + ".csv"
        str_78B = well_78B + string.zfill(4) + ".csv"

        pressure_16A = np.array(pd.read_csv(folder + str_16A, usecols=["pressure"]))
        temper_16A = np.array(pd.read_csv(folder + str_16A, usecols=["temperature"]))
        dispi_16A = np.array(pd.read_csv(folder + str_16A, usecols=["disp_i"]))
        dispj_16A = np.array(pd.read_csv(folder + str_16A, usecols=["disp_j"]))
        dispk_16A = np.array(pd.read_csv(folder + str_16A, usecols=["disp_k"]))

        pressure_56 = np.array(pd.read_csv(folder + str_56, usecols=["pressure"]))
        temper_56 = np.array(pd.read_csv(folder + str_56, usecols=["temperature"]))
        dispi_56 = np.array(pd.read_csv(folder + str_56, usecols=["disp_i"]))
        dispj_56 = np.array(pd.read_csv(folder + str_56, usecols=["disp_j"]))
        dispk_56 = np.array(pd.read_csv(folder + str_56, usecols=["disp_k"]))

        pressure_58 = np.array(pd.read_csv(folder + str_58, usecols=["pressure"]))
        temper_58 = np.array(pd.read_csv(folder + str_58, usecols=["temperature"]))
        dispi_58 = np.array(pd.read_csv(folder + str_58, usecols=["disp_i"]))
        dispj_58 = np.array(pd.read_csv(folder + str_58, usecols=["disp_j"]))
        dispk_58 = np.array(pd.read_csv(folder + str_58, usecols=["disp_k"]))

        pressure_78 = np.array(pd.read_csv(folder + str_78, usecols=["pressure"]))
        temper_78 = np.array(pd.read_csv(folder + str_78, usecols=["temperature"]))
        dispi_78 = np.array(pd.read_csv(folder + str_78, usecols=["disp_i"]))
        dispj_78 = np.array(pd.read_csv(folder + str_78, usecols=["disp_j"]))
        dispk_78 = np.array(pd.read_csv(folder + str_78, usecols=["disp_k"]))

        pressure_78B = np.array(pd.read_csv(folder + str_78B, usecols=["pressure"]))
        temper_78B = np.array(pd.read_csv(folder + str_78B, usecols=["temperature"]))
        dispi_78B = np.array(pd.read_csv(folder + str_78B, usecols=["disp_i"]))
        dispj_78B = np.array(pd.read_csv(folder + str_78B, usecols=["disp_j"]))
        dispk_78B = np.array(pd.read_csv(folder + str_78B, usecols=["disp_k"]))

        data_pressure_16A.append(pressure_16A), data_temper_16A.append(temper_16A)
        data_dispi_16A.append(dispi_16A), data_dispj_16A.append(
            dispj_16A
        ), data_dispk_16A.append(dispk_16A)

        data_pressure_56.append(pressure_56), data_temper_56.append(temper_56)
        data_dispi_56.append(dispi_56), data_dispj_56.append(
            dispj_56
        ), data_dispk_56.append(dispk_56)

        data_pressure_58.append(pressure_58), data_temper_58.append(temper_58)
        data_dispi_58.append(dispi_58), data_dispj_58.append(
            dispj_58
        ), data_dispk_58.append(dispk_58)

        data_pressure_78.append(pressure_78), data_temper_78.append(temper_78)
        data_dispi_78.append(dispi_78), data_dispj_78.append(
            dispj_78
        ), data_dispk_78.append(dispk_78)

        data_pressure_78B.append(pressure_78B), data_temper_78B.append(temper_78B)
        data_dispi_78B.append(dispi_78B), data_dispj_78B.append(
            dispj_78B
        ), data_dispk_78B.append(dispk_78B)

    data_pressure_16A = np.array(data_pressure_16A).reshape(-1, 1)
    data_temper_16A = np.array(data_temper_16A).reshape(-1, 1)
    data_dispi_16A = np.array(data_dispi_16A).reshape(-1, 1)
    data_dispj_16A = np.array(data_dispj_16A).reshape(-1, 1)
    data_dispk_16A = np.array(data_dispk_16A).reshape(-1, 1)

    data_pressure_56 = np.array(data_pressure_56).reshape(-1, 1)
    data_temper_56 = np.array(data_temper_56).reshape(-1, 1)
    data_dispi_56 = np.array(data_dispi_56).reshape(-1, 1)
    data_dispj_56 = np.array(data_dispj_56).reshape(-1, 1)
    data_dispk_56 = np.array(data_dispk_56).reshape(-1, 1)

    data_pressure_58 = np.array(data_pressure_58).reshape(-1, 1)
    data_temper_58 = np.array(data_temper_58).reshape(-1, 1)
    data_dispi_58 = np.array(data_dispi_58).reshape(-1, 1)
    data_dispj_58 = np.array(data_dispj_58).reshape(-1, 1)
    data_dispk_58 = np.array(data_dispk_58).reshape(-1, 1)

    data_pressure_78 = np.array(data_pressure_78).reshape(-1, 1)
    data_temper_78 = np.array(data_temper_78).reshape(-1, 1)
    data_dispi_78 = np.array(data_dispi_78).reshape(-1, 1)
    data_dispj_78 = np.array(data_dispj_78).reshape(-1, 1)
    data_dispk_78 = np.array(data_dispk_78).reshape(-1, 1)

    data_pressure_78B = np.array(data_pressure_78B).reshape(-1, 1)
    data_temper_78B = np.array(data_temper_78B).reshape(-1, 1)
    data_dispi_78B = np.array(data_dispi_78B).reshape(-1, 1)
    data_dispj_78B = np.array(data_dispj_78B).reshape(-1, 1)
    data_dispk_78B = np.array(data_dispk_78B).reshape(-1, 1)

    pressure = np.concatenate(
        (
            data_pressure_16A,
            data_pressure_56,
            data_pressure_58,
            data_pressure_78,
            data_pressure_78B,
        ),
        axis=0,
    )
    temper = np.concatenate(
        (
            data_temper_16A,
            data_temper_56,
            data_temper_58,
            data_temper_78,
            data_temper_78B,
        ),
        axis=0,
    )
    dispi = np.concatenate(
        (data_dispi_16A, data_dispi_56, data_dispi_58, data_dispi_78, data_dispi_78B),
        axis=0,
    )
    dispj = np.concatenate(
        (data_dispj_16A, data_dispj_56, data_dispj_58, data_dispj_78, data_dispj_78B),
        axis=0,
    )
    dispk = np.concatenate(
        (data_dispk_16A, data_dispk_56, data_dispk_58, data_dispk_78, data_dispk_78B),
        axis=0,
    )
    data = np.concatenate((pressure, temper, dispi, dispj, dispk), axis=0)

    return data


## Forward modeling function
def Forward(K_log):
    K_Perm = K_log[0]
    K_Cond = K_log[1]

    K_Perm = np.power(10, K_Perm)
    K_Cond = np.power(10, K_Cond)

    K_Perm = np.format_float_scientific(K_Perm)
    K_Cond = float(K_Cond)

    K_Perm_str = (
        "Materials/permeability_granite/permeability='"
        + str(K_Perm)
        + " 0 0  0 "
        + str(K_Perm)
        + " 0  0 0 "
        + str(K_Perm)
        + "'"
    )

    K_Cond_str = (
        "Materials/thermal_conductivity_granitoid/dry_thermal_conductivity='"
        + str(K_Cond)
        + " 0 0  0 "
        + str(K_Cond)
        + " 0  0 0 "
        + str(K_Cond)
        + "'"
    )

    cli_arg = K_Perm_str + " " + K_Cond_str

    # run simulations
    run_command = (
        "mpiexec -n"
        + " "
        + str(n_cores)
        + " "
        + falcon_run
        + " "
        + "-i"
        + " "
        + falcon_input
        + " "
        + cli_arg
        + " "
        + "--n-threads=2 --timing"
    )
    os.system(run_command)
    simul = Read_data(folder=folder_simul, num_file=num_file)

    return simul


## Delete data produced by previous simulations
def remove_data(folder):
    filePath = folder
    name = os.listdir(filePath)
    for i in name:
        path = "./{}".format(i)
        if "point_data" in i:
            os.remove(path)


## Assume known error matrix R
obs_true = Read_data(folder=folder_ref, num_file=num_file)
np.save(data_f + "/Ref_Data.npy", obs_true)
nobs = len(obs_true)
std_pressure = obs_true[0] * 0.01  # 1% std error
std_temper = obs_true[int(nobs / 5)] * 0.01  # 1% std error
std_dispi = obs_true[int(2 * nobs / 5)] * 0.01  # 1% std error
std_dispj = obs_true[int(3 * nobs / 5)] * 0.01  # 1% std error
std_dispk = obs_true[int(4 * nobs / 5)] * 0.01  # 1% std error
std_pressure = np.abs(std_pressure)
std_temper = np.abs(std_temper)
std_dispi = np.abs(std_dispi)
std_dispj = np.abs(std_dispj)
std_dispk = np.abs(std_dispk)

err_pressure = std_pressure * np.random.randn(int(nobs / 5), 1)
err_temper = std_temper * np.random.randn(int(nobs / 5), 1)
err_dispi = std_dispi * np.random.randn(int(nobs / 5), 1)
err_dispj = std_dispj * np.random.randn(int(nobs / 5), 1)
err_dispk = std_dispk * np.random.randn(int(nobs / 5), 1)
err = np.concatenate(
    (err_pressure, err_temper, err_dispi, err_dispj, err_dispk), axis=0
)
obs = obs_true + err

invR = np.eye(nobs)
for p in range(int(nobs / 5)):
    invR[p, p] = 1 / (std_pressure**2)
for t in range(int(nobs / 5), int(2 * nobs / 5)):
    invR[t, t] = 1 / (std_temper**2)
for i in range(int(2 * nobs / 5), int(3 * nobs / 5)):
    invR[i, i] = 1 / (std_dispi**2)
for j in range(int(3 * nobs / 5), int(4 * nobs / 5)):
    invR[j, j] = 1 / (std_dispj**2)
for k in range(int(4 * nobs / 5), nobs):
    invR[k, k] = 1 / (std_dispk**2)

## Initialization
K_Perm = par.K_Perm
K_Cond = par.K_Cond
K_init = np.vstack((K_Perm, K_Cond))  # initial parameters

std_Perm = 2
std_Cond = 0.05
std_Par = np.vstack((std_Perm, std_Cond))
Sigma = np.eye(nK)
invSigma = np.eye(nK)
for i in range(nK):
    Sigma[i, i] = std_Par[i] ** 2
    invSigma[i, i] = 1 / Sigma[i, i]

precision = par.precision
alpha = par.alpha
K_cur = np.copy(K_init)
Para = np.zeros([max_iter + 1, nK])

## Run inversion
Time_start = time.time()
for i in range(max_iter):
    print("###### Iteration %d ######" % (i + 1))
    remove_data(folder="./")
    simul_K_cur = Forward(K_cur)

    Para[i, 0] = K_cur[0][0]
    Para[i, 1:nK] = K_cur[1:nK].squeeze()
    np.save(data_f + "/Simul_Data_Iter_" + str(i) + ".npy", simul_K_cur)
    np.save(data_f + "/Parameters.npy", Para)

    J_cur = np.zeros([nobs, nK])
    for j in range(nK):
        zerovec = np.zeros([nK, 1])
        zerovec[j] = 1.0
        mag = np.dot(K_cur.T, zerovec)
        absmag = np.dot(abs(K_cur.T), abs(zerovec))
        if mag >= 0:
            signmag = 1.0
        else:
            signmag = -1.0

        delta = (
            signmag
            * np.sqrt(precision)
            * (max(abs(mag), absmag))
            / ((np.linalg.norm(zerovec) + np.finfo(float).eps) ** 2)
        )
        if delta == 0:
            delta = np.sqrt(precision)

        remove_data(folder="./")
        simul_K_delta = Forward(K_cur + zerovec * delta)
        J_cur[:, j : j + 1] = (simul_K_delta - simul_K_cur) / delta

    Jk_cur = np.dot(J_cur, K_cur)
    solve_a = np.dot(np.dot(J_cur.T, invR), J_cur) + invSigma
    solve_b = -np.dot(invSigma, K_cur) + np.dot(
        np.dot(J_cur.T, invR), (obs - simul_K_cur)
    )
    dk = np.linalg.solve(solve_a, solve_b)
    K_cur = np.copy(K_cur + alpha * dk)

## Final results
remove_data(folder="./")
simul_K_cur = Forward(K_cur)

Para[max_iter, 0] = K_cur[0][0]
Para[max_iter, 1:nK] = K_cur[1:nK].squeeze()
np.save(data_f + "/Simul_Data_Iter_" + str(max_iter) + ".npy", simul_K_cur)
np.save(data_f + "/Parameters.npy", Para)

Time_end = time.time()
time_elapsed = Time_end - Time_start
print(
    "Time elapsed {:.0f}hr {:.0f}m {:.0f}s".format(
        (time_elapsed // 60) // 60, (time_elapsed // 60) % 60, time_elapsed % 60
    )
)
