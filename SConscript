Import("env")
env = env.Clone()

sources = [
    "main.c",
    "nonlinear.c",
    "signals.c",
    "util.c",
]
env.Program(target="hammerstein", source=sources)

# test runner
env["LIBS"] += ["python3.12", "dl", "util"]
sources_sans_main = sources.copy()
sources_sans_main.remove("main.c")
sources_sans_main.append("tests.c")
env.Program(target="test_runner", source=sources_sans_main)
