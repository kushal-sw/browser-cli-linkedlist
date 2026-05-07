import re

log_dir = "/Users/kushal/.gemini/antigravity/brain/d2392a4a-4c51-4e0c-80d2-2196133a7be7/.system_generated/logs/overview.txt"

with open(log_dir, "r") as f:
    text = f.read()

# find the last view_file output that had the main class
matches = list(re.finditer(r"The following code has been modified.*?1: #include <iostream>(.*?)// ==================== Main ====================\n\d+: int main\(\) \{(.*?)\*/\n", text, re.DOTALL))
if matches:
    content = "#include <iostream>" + matches[-1].group(1) + "// ==================== Main ====================\nint main() {" + matches[-1].group(2) + "*/\n"
    # remove line numbers
    content = re.sub(r"^\d+: ", "", content, flags=re.MULTILINE)
    
    with open("browser_history.cpp", "w") as f:
        f.write(content)
    print("Restored successfully.")
else:
    print("Could not find backup in logs.")
