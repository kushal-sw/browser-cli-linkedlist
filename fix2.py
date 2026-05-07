import re

log_dir = "/Users/kushal/.gemini/antigravity/brain/d2392a4a-4c51-4e0c-80d2-2196133a7be7/.system_generated/logs/overview.txt"
with open(log_dir, "r") as f:
    text = f.read()

# Let's search for "#include <iostream>"
matches = list(re.finditer(r"1: #include <iostream>.*?742: \*/\n", text, re.DOTALL))
if matches:
    content = matches[-1].group(0)
    # remove line numbers
    content = re.sub(r"^\d+: ", "", content, flags=re.MULTILINE)
    with open("browser_history.cpp", "w") as f:
        f.write(content)
    print("Restored from log!")
else:
    print("Could not find the block.")
