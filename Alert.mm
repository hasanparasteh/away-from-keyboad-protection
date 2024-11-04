// Alert.mm
#import <Cocoa/Cocoa.h>

extern "C" {
void showAlert(const char *message) {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:[NSString stringWithUTF8String:message]];

    // Use default macOS icons by their string names
    NSImage *iconImage = [NSImage imageNamed:@"NSInfo"];
    [alert setIcon:iconImage];

    [alert runModal];
}
}