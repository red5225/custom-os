import importlib.machinery
import importlib.util
import unittest
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "buildroot-external/board/customos/overlay/usr/bin/xc"
LOADER = importlib.machinery.SourceFileLoader("xc", str(SCRIPT))
SPEC = importlib.util.spec_from_loader("xc", LOADER)
xc = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(xc)


class XCTests(unittest.TestCase):
    def test_say_and_variables(self):
        out = []
        variables = xc.run('let score = 10\nadd score 5\nshow score\nfinish', output_fn=out.append)
        self.assertEqual(out, ["15"])
        self.assertEqual(variables["score"], 15)

    def test_if_else(self):
        out = []
        xc.run('let name = "Nolan"\nif name == "Nolan"\n say "yes"\nelse\n say "no"\nend', output_fn=out.append)
        self.assertEqual(out, ["yes"])
        out = []
        xc.run('let name = "Guest"\nif name == "Nolan"\n say "yes"\nelse\n say "no"\nend', output_fn=out.append)
        self.assertEqual(out, ["no"])

    def test_ask(self):
        out = []
        xc.run('ask user\nshow user', input_fn=lambda _: "Ada", output_fn=out.append)
        self.assertEqual(out, ["Ada"])

    def test_unknown_command_is_rejected(self):
        with self.assertRaises(xc.XCError):
            xc.run('delete everything')

    def test_missing_end_is_rejected(self):
        with self.assertRaises(xc.XCError):
            xc.run('if x == 1\n say "hi"')


if __name__ == "__main__":
    unittest.main()
