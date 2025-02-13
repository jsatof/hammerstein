#include <stdio.h>
#include <stdlib.h>

#define PY_SSIZE_T_CLEAN
#include <python3.12/Python.h>

#include <signals.h>

int main(int argc, char **argv) {
    PyStatus py_status;
    PyConfig py_config;
    PyConfig_InitPythonConfig(&py_config);

    py_status = PyConfig_SetBytesString(&py_config, &py_config.program_name, argv[0]);
    if (PyStatus_Exception(py_status)) {
        PyConfig_Clear(&py_config);
        Py_ExitStatusException(py_status);
    }

    py_status = Py_InitializeFromConfig(&py_config);
    if (PyStatus_Exception(py_status)) {
        PyConfig_Clear(&py_config);
        Py_ExitStatusException(py_status);
    }
    PyConfig_Clear(&py_config);

    ///
    /// To run python tests, add the script path to this array
    ///
    char *py_scripts[] = {
        "nonlinear_filter.py",
        "frequency_response.py",
    };
    size_t script_count = sizeof(py_scripts) / sizeof(py_scripts[0]);

    for (size_t i = 0; i < script_count; ++i) {
        FILE *file = fopen(py_scripts[i], "r");
        if (!file) {
            fprintf(stderr, "ERROR: Could not open file %s. Continuing\n", py_scripts[i]);
            continue;
        }
        PyRun_SimpleFile(file, py_scripts[i]);
        fclose(file);
    }




    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
}
