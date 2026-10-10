# CustomOS integrity and recovery

The prototype customos-integrity verifies SHA-256 entries. With an optional recovery directory, it restores only entries whose recovery copy matches the expected hash, using a temporary file and atomic replace. It never deletes user files.

The manifest is not signed. Production recovery needs a signature/trust anchor, versioned recovery media, bounded repair attempts and persistent diagnostics.
