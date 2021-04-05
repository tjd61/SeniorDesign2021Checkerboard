import 'dart:ffi';

import 'package:flutter/material.dart';

const int EvenRow = 0;
const int OddRow = 1;
const Map<int, String> NumToChar = {
  1: 'A',
  2: 'B',
  3: 'C',
  4: 'D',
  5: 'E',
  6: 'F',
  7: 'G',
  8: 'H'
};

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String coordinate1 = "";
  String coordinate2 = "";
  int coordinate1Row;
  int coordinate1Col;
  int coordinate2Row;
  int coordinate2Col;
  String text;
  String move;
  int moveCount = 0;
  int turn = 0;
  bool gameStart = true;
  var arr = new List(8);

  void setCoordinates(int row, int col) {
    String coordinate = "" + NumToChar[row] + col.toString();
    print("setCoordinates()");
    setState(() {
      if (moveCount == 0) {
        this.coordinate1 = coordinate;
        this.coordinate1Row = row;
        this.coordinate1Col = col;
        this.moveCount++;
      } else {
        this.coordinate2 = coordinate;
        this.coordinate2Row = row;
        this.coordinate2Col = col;
        this.moveCount--;
      }
      gameStart = false;
    });
  }

  void setMove() {
    setState(() {
      this.move = this.text;
      if (this.turn !=
          this.arr[this.coordinate1Col - 1][this.coordinate1Row - 1] - 1) {
        return;
      }
      this.arr[this.coordinate1Col - 1][this.coordinate1Row - 1] = 0;
      if (this.turn == 0) {
        this.arr[this.coordinate2Col - 1][this.coordinate2Row - 1] = 1;
        this.turn++;
      } else {
        this.arr[this.coordinate2Col - 1][this.coordinate2Row - 1] = 2;
        this.turn--;
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    text = "Move: (" + this.coordinate1 + " to " + this.coordinate2 + ")";
    if (this.gameStart) {
      arr[0] = [1, 0, 1, 0, 1, 0, 1, 0];
      arr[1] = [0, 1, 0, 1, 0, 1, 0, 1];
      arr[2] = [1, 0, 1, 0, 1, 0, 1, 0];
      arr[3] = [0, 0, 0, 0, 0, 0, 0, 0];
      arr[4] = [0, 0, 0, 0, 0, 0, 0, 0];
      arr[5] = [0, 2, 0, 2, 0, 2, 0, 2];
      arr[6] = [2, 0, 2, 0, 2, 0, 2, 0];
      arr[7] = [0, 2, 0, 2, 0, 2, 0, 2];
    }
    return Scaffold(
        appBar: AppBar(title: Text('Checkers Game')),
        body: Column(
          children: <Widget>[
            RowWidget(this.setCoordinates, EvenRow, 8, arr: arr[7]),
            RowWidget(this.setCoordinates, OddRow, 7, arr: arr[6]),
            RowWidget(this.setCoordinates, EvenRow, 6, arr: arr[5]),
            RowWidget(this.setCoordinates, OddRow, 5, arr: arr[4]),
            RowWidget(this.setCoordinates, EvenRow, 4, arr: arr[3]),
            RowWidget(this.setCoordinates, OddRow, 3, arr: arr[2]),
            RowWidget(this.setCoordinates, EvenRow, 2, arr: arr[1]),
            RowWidget(this.setCoordinates, OddRow, 1, arr: arr[0]),
            NumberRowWidget(),
            Text(this.text),
            Text(""),
            SubmitMoveButtonWidget(this.setMove, this.turn),
            SubmitMoveTextWidget(this.move),
          ],
        ));
  }
}

class RowWidget extends StatefulWidget {
  final int type;
  final int row;
  final Function(int, int) callback;
  final arr;

  RowWidget(this.callback, this.type, this.row, {this.arr});

  @override
  _RowWidgetState createState() => _RowWidgetState();
}

class _RowWidgetState extends State<RowWidget> {
  int piece = 0;
  @override
  Widget build(BuildContext context) {
    if (widget.type == EvenRow) {
      return Row(
        children: <Widget>[
          NumberTileWidget(widget.row.toString()),
          TileWidget(widget.callback, Colors.red, 1, widget.row,
              piece: widget.arr[0]),
          TileWidget(widget.callback, Colors.black, 2, widget.row,
              piece: widget.arr[1]),
          TileWidget(widget.callback, Colors.red, 3, widget.row,
              piece: widget.arr[2]),
          TileWidget(widget.callback, Colors.black, 4, widget.row,
              piece: widget.arr[3]),
          TileWidget(widget.callback, Colors.red, 5, widget.row,
              piece: widget.arr[4]),
          TileWidget(widget.callback, Colors.black, 6, widget.row,
              piece: widget.arr[5]),
          TileWidget(widget.callback, Colors.red, 7, widget.row,
              piece: widget.arr[6]),
          TileWidget(widget.callback, Colors.black, 8, widget.row,
              piece: widget.arr[7]),
        ],
      );
    }

    return Row(
      children: <Widget>[
        NumberTileWidget(widget.row.toString()),
        TileWidget(widget.callback, Colors.black, 1, widget.row,
            piece: widget.arr[0]),
        TileWidget(widget.callback, Colors.red, 2, widget.row,
            piece: widget.arr[1]),
        TileWidget(widget.callback, Colors.black, 3, widget.row,
            piece: widget.arr[2]),
        TileWidget(widget.callback, Colors.red, 4, widget.row,
            piece: widget.arr[3]),
        TileWidget(widget.callback, Colors.black, 5, widget.row,
            piece: widget.arr[4]),
        TileWidget(widget.callback, Colors.red, 6, widget.row,
            piece: widget.arr[5]),
        TileWidget(widget.callback, Colors.black, 7, widget.row,
            piece: widget.arr[6]),
        TileWidget(widget.callback, Colors.red, 8, widget.row,
            piece: widget.arr[7]),
      ],
    );
  }
}

class TileWidget extends StatefulWidget {
  final Color color;
  final int row;
  final int col;
  final Function(int, int) callback;
  final int piece;

  TileWidget(this.callback, this.color, this.row, this.col, {this.piece});

  @override
  _TileWidgetState createState() => _TileWidgetState();
}

class NumberRowWidget extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Row(children: <Widget>[
      NumberTileWidget(''),
      NumberTileWidget('A'),
      NumberTileWidget('B'),
      NumberTileWidget('C'),
      NumberTileWidget('D'),
      NumberTileWidget('E'),
      NumberTileWidget('F'),
      NumberTileWidget('G'),
      NumberTileWidget('H'),
    ]);
  }
}

class _TileWidgetState extends State<TileWidget> {
  void click() {
    int row = widget.row;
    int col = widget.col;
    widget.callback(row, col);
  }

  @override
  Widget build(BuildContext context) {
    if (widget.piece != null && widget.piece > 0) {
      Color color;
      if (widget.piece == 1) {
        color = Colors.red;
      } else {
        color = Colors.blue;
      }
      return Container(
          width: 40,
          height: 40,
          child: FlatButton.icon(
            label: Text(""),
            icon: Icon(Icons.circle),
            color: widget.color,
            padding: new EdgeInsets.all(0.0),
            textColor: color,
            highlightColor: Colors.white,
            onPressed: this.click,
          ));
    }
    return Container(
        width: 40,
        height: 40,
        child: FlatButton(
          child: Text(""),
          color: widget.color,
          highlightColor: Colors.white,
          onPressed: this.click,
        ));
  }
}

class NumberTileWidget extends StatefulWidget {
  final String num;

  NumberTileWidget(this.num);

  @override
  _NumberTileWidgetState createState() => _NumberTileWidgetState();
}

class _NumberTileWidgetState extends State<NumberTileWidget> {
  @override
  Widget build(BuildContext context) {
    return Container(
        width: 40,
        height: 40,
        child: FlatButton(
          child: Text(widget.num),
          color: Colors.white,
          highlightColor: Colors.grey,
          onPressed: () {},
        ));
  }
}

class SubmitMoveButtonWidget extends StatefulWidget {
  final Function() callback;
  final int turn;

  SubmitMoveButtonWidget(this.callback, this.turn);

  @override
  _SubmitMoveButtonWidgetState createState() => _SubmitMoveButtonWidgetState();
}

class _SubmitMoveButtonWidgetState extends State<SubmitMoveButtonWidget> {
  @override
  Widget build(BuildContext context) {
    Color color;
    if (widget.turn == 0) {
      color = Colors.red;
    } else {
      color = Colors.blue;
    }
    return Center(
        child: Container(
            width: 200,
            height: 50,
            child: FlatButton.icon(
              icon: Icon(Icons.keyboard_return),
              label: Text("Enter Move"),
              color: color,
              highlightColor: Colors.green,
              onPressed: widget.callback,
            )));
  }
}

class SubmitMoveTextWidget extends StatefulWidget {
  final String move;

  SubmitMoveTextWidget(this.move);

  @override
  _SubmitMoveTextWidgetState createState() => _SubmitMoveTextWidgetState();
}

class _SubmitMoveTextWidgetState extends State<SubmitMoveTextWidget> {
  @override
  Widget build(BuildContext context) {
    if (widget.move != null) {
      return Center(
          child: Container(
        child: Text("Entered Move: " + widget.move),
      ));
    }
    return Text("");
  }
}
