import sys

def verify(input_file, output_file):
    try:
        with open(input_file, 'r') as f:
            lines = f.readlines()
            if not lines:
                return "FAILED: Input file is empty"
            n = int(lines[0].strip())
            targets = set()
            for i in range(1, n + 1):
                a, b = map(int, lines[i].split())
                targets.add((a, b))
    except Exception as e:
        return f"FAILED: Error reading input file: {e}"

    try:
        with open(output_file, 'r') as f:
            lines = f.readlines()
            if not lines:
                return "FAILED: Output file is empty"
            try:
                m = int(lines[0].strip())
            except ValueError:
                return "FAILED: First line of output is not an integer (M)"
            
            if m > 5 * n:
                return f"FAILED: Too many operations (M={m}, limit={5*n})"
            
            made = set([(0, 0)])
            total_cost = 0
            
            for i in range(1, m + 1):
                if i >= len(lines):
                    return f"FAILED: M={m} but only {len(lines)-1} operations found"
                coords = list(map(int, lines[i].split()))
                if len(coords) != 4:
                    return f"FAILED: Line {i+1} does not have 4 coordinates"
                
                x1, y1, x2, y2 = coords
                if (x1, y1) not in made:
                    return f"FAILED: Starting point ({x1}, {y1}) at move {i} does not exist"
                if x2 < x1 or y2 < y1:
                    return f"FAILED: Invalid move ({x1},{y1}) -> ({x2},{y2}) at move {i}"
                
                total_cost += (x2 - x1) + (y2 - y1)
                made.add((x2, y2))
            
            missing = 0
            for t in targets:
                if t not in made:
                    missing += 1
            
            if missing > 0:
                return f"FAILED: Missing {missing} target(s)"
            
            score = round(10**6 * (n * 10**9) / (1 + total_cost))
            return f"SUCCESS! Cost: {total_cost}, Score: {score}"
            
    except Exception as e:
        return f"FAILED: Error reading output file: {e}"

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 verify.py <input_file> <output_file>")
    else:
        print(verify(sys.argv[1], sys.argv[2]))
