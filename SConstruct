optimization = "-g" if int(ARGUMENTS.get("debug", 1)) else "-O2"
env_options = {
    "LIBS": ["m", "pthread"],
    "CCFLAGS": ["-std=c99", "-Wall", "-Wextra", "-Wpedantic", "-fdiagnostics-color=always", optimization],
    "CPPPATH": [GetLaunchDir()],
}
env = Environment(**env_options)
SConscript("SConscript", exports="env", variant_dir="build", duplicate=False)
