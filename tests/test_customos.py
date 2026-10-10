import hashlib,json,subprocess,sys,tempfile,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
INTEGRITY=ROOT/"buildroot-external/board/customos/overlay/usr/bin/customos-integrity"
APP=ROOT/"buildroot-external/board/customos/overlay/usr/bin/customos-app"
class Tests(unittest.TestCase):
 def run_tool(self,script,*args): return subprocess.run([sys.executable,str(script),*map(str,args)],text=True,capture_output=True)
 def test_app_valid(self):
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)/"a.json"; p.write_text(json.dumps({"name":"demo","version":"1","entry":"main.py","permissions":[]}))
   self.assertEqual(self.run_tool(APP,p).returncode,0)
 def test_app_rejects_traversal(self):
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)/"a.json"; p.write_text(json.dumps({"name":"demo","version":"1","entry":"../evil","permissions":[]}))
   self.assertNotEqual(self.run_tool(APP,p).returncode,0)
 def test_integrity_passes(self):
  with tempfile.TemporaryDirectory() as d:
   root=Path(d)/"root"; root.mkdir(); (root/"s").write_text("good")
   m=Path(d)/"m"; m.write_text(f"{hashlib.sha256(b'good').hexdigest()}  s\n")
   self.assertEqual(self.run_tool(INTEGRITY,m,root).returncode,0)
 def test_integrity_detects_corruption(self):
  with tempfile.TemporaryDirectory() as d:
   root=Path(d)/"root"; root.mkdir(); (root/"s").write_text("bad")
   m=Path(d)/"m"; m.write_text(f"{hashlib.sha256(b'good').hexdigest()}  s\n")
   self.assertEqual(self.run_tool(INTEGRITY,m,root).returncode,1)
 def test_integrity_restores(self):
  with tempfile.TemporaryDirectory() as d:
   root=Path(d)/"root"; root.mkdir(); rec=Path(d)/"rec"; rec.mkdir()
   (root/"s").write_text("bad"); (rec/"s").write_text("good")
   m=Path(d)/"m"; m.write_text(f"{hashlib.sha256(b'good').hexdigest()}  s\n")
   self.assertEqual(self.run_tool(INTEGRITY,m,root,rec).returncode,1)
   self.assertEqual((root/"s").read_text(),"good")
if __name__=="__main__": unittest.main()
