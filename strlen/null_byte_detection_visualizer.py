from termcolor import colored

class StrlenVisualizer:
    HIMAGIC: int = 0x8080
    LOMAGIC: int = 0x0101

    def __init__(self):
        pass

    def visualize(self, w: int, wsize: int = 4) -> None:
        max_w_value = pow(256, wsize) - 1
        lw_lomagic = (w - self.LOMAGIC) & max_w_value
        lw_not = ~w & max_w_value
        lw_not_himagic = lw_not & self.HIMAGIC
        result = lw_lomagic & lw_not & lw_not_himagic & max_w_value


        def tobin(num: int) -> str:
            return bin(num)[2:].zfill(16)
        
        def tohex(num: int) -> str:
            hex_str = hex(num)[2:] 
            return "0x" + hex_str.zfill(4)       

        def code(s: str) -> str:
            return colored(s, "grey")

        def prompt(step: str):
            input(colored(f"\nPress Enter to continue to {step}...\n", "cyan"))

        def intro():
            print(colored("Welcome to the strlen null byte detection visualizer!", "yellow"))
            print("This tool will help you understand the optimized technique used in GLIBC's strlen function.")
            print()
            prompt("the introduction")

            print("The goal of this optimization is to detect null bytes in a whole word instead of checking one byte at a time.")
            print()
            print(f"{code('A word is a sequence of bytes of <CPU-Architecture> bits length.')}")
            print()
            print("To achieve this, we use two magic values:")
            print(f"1. {code('himagic')} - MSB of each byte set: {code(tobin(self.HIMAGIC))}")
            print(f"2. {code('lomagic')} - LSB of each byte set: {code(tobin(self.LOMAGIC))}")
            print()
            print(f"We'll analyze how these values help detect null bytes in {code('w')}:")
            print(f"{code(tobin(w))}")

        def step1():
            print(colored("Step 1: Subtracting lomagic from w", "yellow"))
            print(f"The subtraction {code('w - lomagic')} causes any {code('0x00')} byte in {code('w')} to underflow to {code('0xFF')}, while also making sure the MSB of any byte {code('0x01 <= b <= 0x80')} is cleared.")
            print(f"    {code(tobin(w))} // {code(tohex(w))}")
            print(f"  - {code(tobin(self.LOMAGIC))} // {code(tohex(self.LOMAGIC))}")
            print("    ________________________________")
            print(f"    {code(tobin(lw_lomagic))} // {code(tohex(lw_lomagic))}")

        def step2():
            print(colored("Step 2: Applying bitwise NOT to w.", "yellow"))
            print(f"The bitwise NOT (~) flips all bits in {code('w')}, turning any {code('0x00')} byte into {code('0xFF')}, while clearing the MSB of any byte {code('0x81 <= b <= 0xFF')}.")
            print(f"  ~ {code(tobin(w))} // {code(tohex(w))}")
            print("    ________________________________")
            print(f"    {code(tobin(lw_not))} // {code(tohex(lw_not))}")

        def step3():
            print(colored("Step 3: Final condition", "yellow"))
            print(f"The final step combines the results of all previous operations:")
            print(f"    {code(tobin(lw_lomagic))} // {code(tohex(lw_lomagic))}")
            print(f"  & {code(tobin(lw_not))} // {code(tohex(lw_not))}")
            print(f"  & {code(tobin(lw_not_himagic))} // {code(tohex(lw_not_himagic))}")
            print("    ________________________________")
            print(f"    {code(tobin(result))} // {code(tohex(result))}")
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

if __name__ == "__main__":
    visualizer = StrlenVisualizer()
    print(colored("Enter a 16-bit hexadecimal value (e.g., 0x1200):", "cyan"))
    user_input = input(">> ").strip()
    try:
        w = int(user_input, 16)
        visualizer.visualize(w, 2)
    except ValueError:
        print(colored("Invalid input! Please enter a valid 32-bit hexadecimal value.", "red"))
