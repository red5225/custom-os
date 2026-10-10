# CustomOS UI language

The first Linux boot milestone does not ship a graphical renderer. The UI language will be a small declarative format parsed by a native service and rendered by the desktop shell, not by a browser.

Proposed syntax:

    window "Hello" {
      title: "Hello CustomOS"
      width: 480
      height: 280
      text "Welcome to CustomOS"
      button "Open terminal" action: "system.launch:terminal"
    }

This is a design proposal, not a claimed working renderer.
