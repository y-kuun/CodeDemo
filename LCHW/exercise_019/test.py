# coding: utf8
import os
from random import randint
from random import choice

target_program = 'ex19'
test_input_time = 100000
simu_cmd_file = 'cmd.txt'

def test_white(exe_name):
	cmd_set = 'n s e w a 1'.split()
	cmd_array = []
	for i in range(test_input_time):
		cmd_array.append(choice(cmd_set))
	cmd_array.append('%s\n' % chr(255))	

	with open(simu_cmd_file, 'wb') as f:
		f.write('\n'.join(cmd_array))

	cmd = "./{exe} < {cmd}".format(cmd=simu_cmd_file,exe=target_program)
	# cmd = "cat {cmd} | ./{exe}".format(cmd=simu_cmd_file,exe=target_program)
	print cmd
	os.system(cmd)

if __name__ == '__main__':
	# test(target_program)
	test_white(target_program)
