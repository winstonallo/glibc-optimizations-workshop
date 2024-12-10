from termcolor import colored

class StrlenVisualizer:
    HIMAGIC: int = 0x80808080
    LOMAGIC: int = 0x01010101

    def __init__(self):
        pass

    def visualize(self, longword: int) -> None:
        lw_lomagic = (longword - self.LOMAGIC) & 0xFFFFFFFF
        lw_not = ~longword & 0xFFFFFFFF
        lw_not_himagic = lw_not & self.HIMAGIC
        result = lw_lomagic & lw_not & lw_not_himagic & 0xFFFFFFFF


        def tobin(num: int) -> str:
            return bin(num)[2:].zfill(32)

        def code(s: str) -> str:
            return colored(s, "grey")

        def prompt(step: str):
            input(colored(f"\nPress Enter to continue to {step}...\n", "cyan"))

        def intro():
            print(colored("Welcome to the strlen null byte detection visualizer!", "yellow"))
            print("This tool will help you understand the optimized technique used in GLIBC's strlen function.")
            print()
            prompt("the introduction")

            print("The goal of this optimization is to detect null bytes in 'words' (4 bytes in this case) instead of checking one byte at a time.")
            print()
            print(f"{code('A word is a sequence of bytes of <CPU-Architecture> bits length. Here, we use 32-bit mode for simplicity.')}")
            print()
            print("To achieve this, we use two magic values:")
            print(f"1. {code('himagic')} - MSB of each byte set: {code(tobin(self.HIMAGIC))}")
            print(f"2. {code('lomagic')} - LSB of each byte set: {code(tobin(self.LOMAGIC))}")
            print()
            print(f"We'll analyze how these values help detect null bytes in {code('longword')}:")
            print(f"{code(tobin(longword))}")

        def step1():
            print(colored("Step 1: Subtracting lomagic from longword", "yellow"))
            print(f"The subtraction {code('longword - lomagic')} causes any {code('0x00')} byte in {code('longword')} to underflow to {code('0xFF')}.")
            print(f"    {code(tobin(longword))}")
            print(f"  - {code(tobin(self.LOMAGIC))}")
            print("    ________________________________")
            print(f"    {code(tobin(lw_lomagic))}")

        def step2():
            print(colored("Step 2: Applying bitwise NOT to longword", "yellow"))
            print(f"The bitwise NOT (~) flips all bits in {code('longword')}:")
            print(f"  ~ {code(tobin(longword))}")
            print("    ________________________________")
            print(f"    {code(tobin(lw_not))}")

        def step3():
            print(colored("Step 3: Combining NOT(longword) with himagic", "yellow"))
            print(f"By combining {code('~longword')} with {code('himagic')}, only MSBs of bytes with {code('0x00')} are preserved:")
            print(f"    {code(tobin(lw_not))}")
            print(f"  & {code(tobin(self.HIMAGIC))}")
            print("    ________________________________")
            print(f"    {code(tobin(lw_not_himagic))}")

        def step4():
            print(colored("Step 4: Final condition", "yellow"))
            print(f"The final step combines the results of all previous operations:")
            print(f"    {code(tobin(lw_lomagic))}")
            print(f"  & {code(tobin(lw_not))}")
            print(f"  & {code(tobin(lw_not_himagic))}")
            print("    ________________________________")
            print(f"    {code(tobin(result))}")
            print()
            if result != 0:
                print(colored("Result: A null byte was detected in the word!", "green"))
            else:
                print(colored("Result: No null byte was detected in the word.", "red"))

        intro()
        prompt("Step 1")
        step1()
        prompt("Step 2")
        step2()
        prompt("Step 3")
        step3()
        prompt("Step 4")
        step4()

if __name__ == "__main__":
    visualizer = StrlenVisualizer()
    print(colored("Enter a 32-bit hexadecimal value (e.g., 0x12345611):", "cyan"))
    user_input = input(">> ").strip()
    try:
        longword = int(user_input, 16)
        visualizer.visualize(longword)
    except ValueError:
        print(colored("Invalid input! Please enter a valid 32-bit hexadecimal value.", "red"))
