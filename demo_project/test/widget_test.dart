import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';

import 'package:demo_project/main.dart';

void main() {
  testWidgets('Given start of game Then there are 24 pieces',
      (WidgetTester tester) async {
    // ARRANGE
    await tester.pumpWidget(MyApp());

    // ASSERT
    expect(find.byIcon(Icons.circle), findsNWidgets(24));
  });

  testWidgets('Given start of game Then there is a move input button',
      (WidgetTester tester) async {
    // ARRANGE
    await tester.pumpWidget(MyApp());

    // ASSERT
    expect(find.text("Enter Move"), findsOneWidget);
  });
}
