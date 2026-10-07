# NOL — Native OS Language

NOL is the programming language being built specifically for CustomOS.

The goal is for CustomOS to eventually be implemented mostly in NOL, with only tiny architecture-specific pieces remaining in assembly and hardware glue.

Example:

fn hello() {
    print "hello\n";
}

The bootstrap compiler currently translates NOL into freestanding C during the build. This is temporary. The planned compiler is a real NOL compiler with its own intermediate representation and native backend.
