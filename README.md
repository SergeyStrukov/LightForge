# LightForge

add <path> ( <build> )* // empty list = std
del <path> ( <build> )* // empty list = all


Build/Project/Target /obj /dep

*.a
*.exe | loader *.exe
pregen *.exe -> file

## PROJECT

name: name1 ... nameN

// project names

## TARGET

(exe|lib|pregen) name: name1 ... nameN

// target names   target OR proj.target

OUT = name

SRC = dir1 ... dirN

INC = dir1 ... dirN

INC_PRIVATE = dir1 ... dirN


