using System.Text.Json;

static int Fail(string message)
{
    Console.Error.WriteLine($"manifest-check: {message}");
    return 2;
}

static bool ValidManifest(string json)
{
    try
    {
        using var doc = JsonDocument.Parse(json);
        var root = doc.RootElement;
        if (root.ValueKind != JsonValueKind.Object) return false;
        if (!root.TryGetProperty("name", out var name) || name.ValueKind != JsonValueKind.String || string.IsNullOrWhiteSpace(name.GetString())) return false;
        if (!root.TryGetProperty("version", out var version) || version.ValueKind != JsonValueKind.String || string.IsNullOrWhiteSpace(version.GetString())) return false;
        if (!root.TryGetProperty("entry", out var entry) || entry.ValueKind != JsonValueKind.String) return false;
        var path = entry.GetString() ?? "";
        if (string.IsNullOrWhiteSpace(path) || Path.IsPathRooted(path) || path.Split('/', '\\').Any(part => part == "..")) return false;
        if (!root.TryGetProperty("permissions", out var permissions) || permissions.ValueKind != JsonValueKind.Array) return false;
        return permissions.EnumerateArray().All(p => p.ValueKind == JsonValueKind.String);
    }
    catch (JsonException) { return false; }
}

if (args.Length == 1 && args[0] == "--self-test")
{
    const string valid = "{\"name\":\"demo\",\"version\":\"1\",\"entry\":\"main.xc\",\"permissions\":[]}";
    const string invalid = "{\"name\":\"demo\",\"version\":\"1\",\"entry\":\"../escape\",\"permissions\":[]}";
    if (!ValidManifest(valid) || ValidManifest(invalid)) return Fail("self-test failed");
    Console.WriteLine("C# manifest validator: PASS");
    return 0;
}
if (args.Length != 1) return Fail("usage: ManifestCheck <manifest.json> | --self-test");
string content;
try { content = File.ReadAllText(args[0]); }
catch (IOException ex) { return Fail(ex.Message); }
if (!ValidManifest(content)) return Fail("invalid manifest or unsafe entry path");
Console.WriteLine("manifest: PASS");
return 0;
