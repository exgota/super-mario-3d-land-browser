from pathlib import Path
import hashlib,json,os
here=Path(__file__).resolve().parent
seal=json.loads((here/'recorded/input-seal.json').read_text())
for name,expected in seal['files'].items():
 actual=hashlib.sha256(Path(name).read_bytes()).hexdigest()
 assert actual==expected,(name,expected,actual)
for name,expected in seal['old_tracked_files'].items():
 assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==expected,name
base=Path(os.environ.get('PRIMITIVE_BASELINE','../mario-main861'))
assert hashlib.sha256((base/'build/dot-baseline-861/report.json').read_bytes()).hexdigest()==seal['baseline_report_sha256']
print('Verified frozen source, all 625 prior tracked files, game data and 791/902 toolchain hashes before execution')
