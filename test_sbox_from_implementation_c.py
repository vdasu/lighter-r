
def MAOI1(a, b, c, d):
	assert a == c and b == d
	return a^b

def MOAI1(a, b, c, d):
	assert a == c and b == d
	return not (a^b)

def NOT1(a):
	return not a

def OR3(a, b, c):
	return a | b | c

def NOR2(a, b):
	return not (a | b)

def NOR3(a, b, c):
	return NOT1(OR3(a, b, c))

def NAND2(a, b):
	return not (a & b)

def XOR2(a, b):
	return (a ^ b)

def XNOR2(a, b):
	return not (a ^ b)

def OR2(a, b):
	return (a | b)

def AND2(a, b):
	return (a & b)	

def AND3(a, b, c):
	return a & b & c

def ANDN2(a, b):
	return NOT1(a) & (b)

######### Reversible 

def RNOT1(a):
	return not a 

def CNOT1(a, b):
	return a ^ b

def CCNOT2(a, b, c):
	return (a & b) ^ c
	#return a ^ (b & c)

def CCCNOT2(a, b, c, d):
	return (a & b & c) ^ d


def FRED1(a, b, c):
	return ((not a) & b) | (a & c)

def FRED2(a, b, c):
	return ((not a) & c) | (a & b)

Z = []
for f in range(16)[::]:

	Y = [None]*4


	X = [x == '1' for x in list((bin(f)[2:]).zfill(4))]
	F = [None]*4

	############ PRINCE SBox (BF32AC916780E5D4)
    ########### Copy starts
	F[0] = X[2];
	F[1] = X[1];
	F[2] = X[0];
	F[3] = X[3];

	F[1] = CNOT1(F[1], F[0]);
	F[2] = CCNOT2(F[1], F[0], F[2]);
	F[0] = CCNOT2(F[2], F[1], F[0]);
	F[3] = CCNOT2(F[2], F[0], F[3]);
	F[2] = CCNOT2(F[3], F[0], F[2]);
	F[1] = CCNOT2(F[3], F[2], F[1]);
	F[0] = RNOT1(F[0]);
	F[3] = CNOT1(F[3], F[2]);
	F[3] = CCCNOT2(F[0], F[1], F[2], F[3]);
	F[2] = RNOT1(F[2]);
	F[0] = CCCNOT2(F[1], F[2], F[3], F[0]);
	F[1] = CNOT1(F[1], F[2]);

	X[0] = F[2];
	X[1] = F[3];
	X[2] = F[0];
	X[3] = F[1];                                                                                                                                                         
    ########### Copy ends

	Y[:] = X[:]
	Y = (int('0b'+(''.join(map(str, map(int, Y[::])))), 2))
	
	Z.append(Y)
	

for f in range(16):
	if f not in Z:
		print ('Error, missing %x'%f)
print (''.join(map(lambda x: hex(x)[2:], Z))).upper()
