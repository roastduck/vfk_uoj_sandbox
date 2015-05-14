.PHNOY : install

uoj_run : uoj_env.h run_program_conf.h run_program.cpp
	g++ run_program.cpp -o uoj_run -O2

install : uoj_run
	cp uoj_run /usr/bin/
