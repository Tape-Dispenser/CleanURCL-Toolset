import sys,os,time
from clean import *
from transpile import *
from assemble import *

def print_info():
    print('bruh more args pls') # TODO: make this print actual info

command = ""
f = ""
args = []

# cli handling shit
if __name__ == "__main__":
    if len(sys.argv) == 1:
        print_info()
        exit()
    for i, arg in enumerate(sys.argv):
        if i == 0:
            pass
        elif i == 1:
            command = arg.lower()
        elif i == 2:
            if os.path.isfile(arg):
                f = arg
            else:
                args.append(arg)
        else:
            if os.path.isfile(arg):
                args.append(arg)
            else:
                args.append(arg)


#parse command
match command.lower():
    case 'clean':
        startTime = time.perf_counter_ns()
        x = clean(f,args)
        if x == 'error':
            exit()
        if os.path.isfile(x):
            endTime = time.perf_counter_ns()
            print(f"file successfully cleaned and outputted to \"{x}\" in {endTime-startTime} nanoseconds.")
    
    case 'transpile':
        startTime = time.perf_counter_ns()
        x = transpile(f,args)
        if os.path.isfile(x):
            endTime = time.perf_counter_ns()
            print(f"file successfully transpiled and outputted to \"{x}\" in {endTime-startTime} nanoseconds.")

    case 'assemble':
        startTime = time.perf_counter_ns()
        x = assemble(f,args)
        if os.path.isfile(x):
            endTime = time.perf_counter_ns()
            print(f"file successfully assembled and outputted to \"{x}\" in {endTime-startTime} nanoseconds.")