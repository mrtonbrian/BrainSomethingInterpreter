## BrainSomething Interpreter
Interprets the Esolang Brainf**k. Implements some [peephole optimizations.](https://en.wikipedia.org/wiki/Peephole_optimization)

Some benchmarks:
|                            | mandelbrot.b | hanoi.b |
|----------------------------|--------------|---------|
| Basic interpreter          | 29.4s        | 12.1s   |
| After operator compression | 11.3s        | 15.7s   |
| Operation compression + removing virtual functions | 7.2s | 7.6s |
| After removing the tape class | 6.6s | 7.2s |
| Adding clear command | 6.7s | 0.6s |

cat.b: From https://esolangs.org/wiki/Brainfuck

hanoi.b: From https://github.com/fabianishere/brainfuck/blob/master/examples/hanoi.bf

mandelbrot.b: From https://github.com/erikdubbelboer/brainfuck-jit/blob/master/mandelbrot.bf

helloworld.b: From https://therenegadecoder.com/code/hello-world-in-brainfuck/