import os

def info():
    print('Assembles URCL code')
    print('')
    print('Available options:')
    print('\t--o <file_path> : declare output file path (default is URCL.py/output/a.out)')
    print('')
    print('\t-h : display this message')
    return

OpcodeDict = {
    # tuple[0] = opcode
    # tuple[1] = operand count
    'add':   (1, 3),
    'rsh':   (9, 2),
    'lod':   (4, 2),
    'str':   (5, 2),
    'bge':   (3, 3),
    'nor':   (8, 3),
    'sub':   (10, 3),
    'jmp':   (23, 1),
    'mov':   (11, 2),
    'nop':   (22, 0),
    'imm':   (2, 1),
    'lsh':   (12, 2),
    'inc':   (13, 2),
    'dec':   (14, 2),
    'neg':   (15, 2),
    'and':   (16, 3),
    'or':    (17, 3),
    'not':   (18, 2),
    'xnor':  (19, 3),
    'xor':   (20, 3),
    'nand':  (21, 3),
    'brl':   (24, 3),
    'brg':   (25, 3),
    'bre':   (26, 3),
    'bne':   (27, 3),
    'bod':   (28, 2),
    'bev':   (29, 2),
    'ble':   (30, 3),
    'brz':   (31, 2),
    'bnz':   (32, 2),
    'brn':   (33, 2),
    'brp':   (34, 2),
    'psh':   (39, 1),
    'pop':   (40, 1),
    'cal':   (37, 1),
    'ret':   (38, 0),
    'hlt':   (0, 0),
    'cpy':   (41, 2),
    'brc':   (35, 3),
    'bnc':   (36, 3),

    # complex
    'mlt':   (42, 3),
    'div':   (43, 3),
    'sdiv':  (44, 3),
    'mod':   (45, 3),
    'bsr':   (46, 3),
    'bsl':   (47, 3),
    'srs':   (48, 2),
    'bss':   (49, 3),
    'sbrl':  (62, 3),
    'sbrg':  (63, 3),
    'sble':  (64, 3),
    'sbge':  (65, 3),
    'sete':  (50, 3),
    'setne': (51, 3),
    'setg':  (52, 3),
    'setl':  (53, 3),
    'setge': (54, 3),
    'setle': (55, 3),
    'setc':  (56, 3),
    'setnc': (57, 3),
    'ssetg': (58, 3),
    'ssetl': (59, 3),
    'ssetge':(60, 3),
    'ssetle':(61, 3),
    'llod':  (66, 3),
    'lstr':  (67, 3),
    
    
    'in':    (6, 2),
    'out':   (7, 2)
}






def assemble(file, options=[]):
    outFile = './output/out.urcl'                                       # default output file
    if file == "":                                                      # if file is blank just print info and exit
        info()
        return
    
    endCommand = ""
    for c,v in enumerate(options):                          
        if v.lower() == '--o':
            try:
                outFile = options[c+1].strip()                                  # argument immediately after -o should be a file path
                if not (os.path.isfile(outFile)):
                    raise Exception(f'Error parsing command: "{outFile}" is not a valid file path')  
            except IndexError:
                raise Exception('Error parsing command: -o flag used but no output path provided')
        
        if v.lower() == '-h':
            info()
            return
        if v.lower() == '-c':
            compiler_extras = False

        if v.lower() == '-e':
            try:
                endCommand = options[c+1]
            except IndexError:
                raise Exception('Error parsing command: -e flag used but no command provided')
            
    if os.path.isfile(file):
        with open(file, 'r') as f:
            codeList = f.readlines()
    else:
        raise Exception('Error parsing command: no source file provided/invalid file path.')


    # there needs to be a check for whether or not the code is cleaned    
    for line in codeList:
        print(line)

