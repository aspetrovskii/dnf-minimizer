TEMPLATE = subdirs
SUBDIRS += minimizer_core dnf_minimizer_app
minimizer_core.file = backend/minimizer_core.pro
dnf_minimizer_app.file = src/dnf_minimizer.pro
dnf_minimizer_app.depends = minimizer_core
