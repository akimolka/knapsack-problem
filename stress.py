import os
import random

stupid = "standard"
smart = "main"
gen = "gen"

class Rational:
    def __init__(self, numerator, denominator):
        self.numerator = numerator
        self.denominator = denominator


def RatioGreaterThanOnePlusEpsilon(numerator, denominator, epsilon):
    # returns numerator / denominator > (1 + epsilon)
    if epsilon is Rational:
        return numerator * epsilon.denominator > (epsilon.numerator + epsilon.denominator) * denominator
    else:
        return numerator > denominator * (1 + epsilon)


def compile(a):
    return "g++ -std=c++17 ./" + a + ".cpp -o " + a

def main():
    os.system("clear && " + compile(stupid) + " && " + compile(smart) + " && " + compile(gen))
    for te in range(1000000):
        precision = random.random()
        os.system("./" + gen + " " + str(te) + f" n=100 max_weight=500 cost=10000 precision={precision} " + " > in")
        RE1 = os.system("./" + stupid + " < in > out")
        ans = int(open("out", 'r').read())
        RE2 = os.system("./" + smart + " < in > out")
        out = int(open("out", 'r').read())

        flag = 0

        if RE1:
            flag = 1
            print("RE " + stupid)
        if RE2:
            flag = 1
            print("RE " + smart)
        if RatioGreaterThanOnePlusEpsilon(ans, out, precision):
        # if ans != out:
            flag = 1
            print("WA")
            print("Correct ans:", ans, sep="\n")
            print("Output:", out, sep="\n")
        if flag == 1:
            print("Input:", open("in", 'r').read(), sep="\n")
            break
        print("OK", te)

main()