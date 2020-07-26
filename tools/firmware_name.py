Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
print(defines)

env.Replace(PROGNAME="ESPbutton%s_v%s" % (defines.get("USE_SSL") or "", defines.get("VERSION") or "0b"))