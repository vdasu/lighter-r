
# Run with Python 2
# Functionality to convert the output of an implementation file which is output from LIGHTER (in C-like syntax) to a TFC [http://webhome.cs.uvic.ca/~dmaslov/mach-read.html] file

file_name = 'test.c'
impl_file = open(file_name, 'r')
tfc_file = open(impl_file.split('.')[0]+'tfc', 'w')

print >> tfc_file, 
