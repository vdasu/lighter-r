
# Run with Python 2
# Basic implementation
# Functionality to convert the output of an implementation file which is output from LIGHTER (in C-like syntax) to a TFC [http://webhome.cs.uvic.ca/~dmaslov/mach-read.html] file

file_name = 'test.c'
impl_file = open(file_name, 'r')
tfc_file = open(file_name.split('.')[0]+'.tfc', 'w')
varible = 'x'
opening_msg = 'This file is auto-generated'
print >> tfc_file, '# '+opening_msg

# copy the content
for f in impl_file.readlines():
	f = f.strip()
	print >> tfc_file, '# '+f


def get_numbers(x, varible=varible):
	assert type(x) == type(' ')
	assert x.find('F') != -1
	x = x.replace('F', '').replace('[', '').replace(']', '')
	assert len(x) >= 1
	if len(x) == 1:
		assert str.isdigit(x)
		return varible+'_'+(x)
	else:
		y = x.split(',')
		assert [str.isdigit(yy) for yy in y]
		return ','.join([varible+'_'+xx for xx in y])

impl_file.seek(0)
initial_ordering = []
print_str = []
final_ordering = [] # the final permutation of bits is not supported in RCViwer+
for f in impl_file.readlines():
	f = f.strip()
	# ignore comments or blank lines
	if f.startswith('//') or f.strip() == '':
		pass
	# initial and final bit permutations
	elif f.find('(') == -1: 
				
		d = ((f.replace(' ', '').replace(';', '')).split('=')[1])[-2]
		assert str.isdigit(d)
		d = int(d)
		assert d in range(0, 4)
		if len(initial_ordering) <= 3:
			initial_ordering.append(d)
		else:
			final_ordering.append(d)
	else:
		f = f.replace(' ', '').replace(';','').replace(')','')
		exp = f.split('=')[1].split('(')

		if exp[0] == 'RNOT1':
			print_str.append('t1'+' '+ get_numbers(exp[1]))
		elif exp[0] == 'CCNOT2':
			print_str.append('t3'+' '+ get_numbers(exp[1]))
		#else:
		#	raise NotImplementedError
print >> tfc_file, ''
print >> tfc_file, '.v'+' '+ ','.join([varible+'_'+str(i) for i in initial_ordering])
print >> tfc_file, '.i'+' '+ ','.join([varible+'_'+str(i) for i in initial_ordering])
print >> tfc_file, '.o'+' '+ ','.join([varible+'_'+str(i) for i in final_ordering])
print >> tfc_file, 'BEGIN'
for p in print_str:
	print >> tfc_file, p+''
print >> tfc_file, 'END'
impl_file.close()
tfc_file.close()	
