"""Generate deterministic synthetic CAN IDS data for offline experiments."""
import csv
import sys

def main(out_path):
    rows=[]
    t=0
    for n in range(1000):
        ident=0x100 if n%2==0 else 0x200
        payload=bytes(((n+i*17)&0xFF) for i in range(8))
        rows.append([t, f"0x{ident:03X}", 8, payload.hex(), "normal"])
        t+=10000
    rows.append([t, "0x666", 8, "123456789abcdef0", "id_injection"])
    t+=10000
    rows.append([t, "0x100", 4, "12345678", "dlc_tamper"])
    t+=10000
    rows.append([t, "0x200", 8, "aabbccddeeff0011", "payload_tamper"])
    with open(out_path,"w",newline="",encoding="utf-8") as f:
        w=csv.writer(f); w.writerow(["timestamp_us","id","dlc","data_hex","scenario"]); w.writerows(rows)
    print(f"wrote {len(rows)} frames to {out_path}")

if __name__=="__main__":
    if len(sys.argv)!=2:
        print("usage: python3 tools/generate_attack_data.py output.csv"); raise SystemExit(2)
    main(sys.argv[1])
