import os
import re

dirname = os.path.abspath(os.path.dirname(__file__))


def exec(command):
    print('>>>', command)
    os.system(command)


def main():
    with open(os.path.join(dirname, 'src/template.tex'), 'r+', encoding='utf8') as f:
        template = f.read()
    with open(os.path.join(dirname, 'src/TOC.txt'), 'r+', encoding='utf8') as f:
        toc = f.read()

    content = ''
    root = os.path.join(dirname, 'src')

    def gen(current, level, dir):
        nonlocal content
        l = 0
        while l < len(current):
            r = l
            while r + 1 < len(current) and current[r + 1].startswith('\t'):
                r += 1
            print(l, r, current[l:r + 1])

            name = current[l]
            content += f'\{level}{{{name}}}\n'

            getname = lambda extname: os.path.join(dir, name + '.' + extname).replace('\\', '/')
            if os.path.isfile(os.path.join(root, dir, name + '.tex')):
                content += f'\input{{{getname("tex")}}}\n'
            if os.path.isfile(os.path.join(root, dir, name + '.py')):
                content += f'\inputminted{{python}}{{{getname("py")}}}\n'
            if os.path.isfile(os.path.join(root, dir, name + '.cpp')):
                srcfile = os.path.join(root, dir, name + '.cpp')
                dstfile = os.path.join(root, 'tmp', dir, name + '.cpp')
                with open(srcfile, 'r+', encoding='utf8') as f:
                    code = f.read()
                    match = re.search(r'// poplib start\n(.*?)\n// poplib end', code, re.DOTALL)
                    if match:
                        code = match.group(1)
                if not os.path.exists(os.path.dirname(dstfile)):
                    os.makedirs(os.path.dirname(dstfile))
                with open(dstfile, 'w+', encoding='utf8') as f:
                    f.write(code)
                content += f'\inputminted{{cpp}}{{tmp/{getname("cpp")}}}\n'
            elif os.path.isfile(os.path.join(root, dir, name + '.hpp')):
                content += f'\inputminted{{cpp}}{{{getname("hpp")}}}\n'

            sublist = current[l + 1:r + 1]
            for i in range(len(sublist)):
                sublist[i] = sublist[i][1:]
            gen(sublist, f'sub{level}', os.path.join(dir, name))
            l = r + 1

    gen(toc.split('\n'), 'section', '')
    print(content)

    with open(os.path.join(dirname, 'src/main.tex'), 'w+', encoding='utf8') as f:
        template = template.split('%==================== SECTIONS ====================%')
        f.write(template[0] + content + template[2])

    os.chdir(os.path.join(dirname, 'src'))
    os.system('xelatex -shell-escape main.tex')
    os.system('xelatex -shell-escape main.tex')


if __name__ == '__main__':
    main()