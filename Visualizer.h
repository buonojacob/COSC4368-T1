#ifndef VISUALIZER_H
#define VISUALIZER_H

#pragma once
#include "raylib.h"
#include "QTable.h"
#include "World.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

// assisted Visualizer class

// encodes a state from components
static int encodeState(int i, int j, bool holding, bool s, bool t, bool u) {
    string combined = to_string(i) + to_string(j) + to_string(holding ? 1 : 0)
                    + to_string(s ? 1 : 0) + to_string(t ? 1 : 0) + to_string(u ? 1 : 0);
    return stoi(combined);
}

// maps a Q-value to a color: dark red = negative and dark green = positive
static Color qValueColor(double val, double minVal, double maxVal) {
    // normalize to [0, 1]
    double range = maxVal - minVal;
    double t = (range == 0.0) ? 0.5 : (val - minVal) / range;
    t = max(0.0, min(1.0, t));

    // dark red to dark grey to dark green
    if (t < 0.5) {
        float f = (float)(t * 2.0);
        return { (unsigned char)(139),
                 (unsigned char)(f * 100),
                 (unsigned char)(f * 20),
                 255 };
    } else {
        float f = (float)((t - 0.5) * 2.0);
        return { (unsigned char)(139 - f * 100),
                 (unsigned char)(100 + f * 100),
                 (unsigned char)(20),
                 255 };
    }
}

static string fmtQ(double v) {
    ostringstream oss;
    oss << fixed << setprecision(2) << v;
    return oss.str();
}

// draws a triangle and its Q-value label inside a cell
// dir: 0=N, 1=S, 2=W, 3=E
static void drawDirectionTriangle(int cx, int cy, int half, int dir,
                                   double qVal, double minVal, double maxVal,
                                   Font font, int fontSize) {
    Color col = qValueColor(qVal, minVal, maxVal);

    Vector2 center = { (float)cx, (float)cy };
    Vector2 topLeft     = { (float)(cx - half), (float)(cy - half) };
    Vector2 topRight    = { (float)(cx + half), (float)(cy - half) };
    Vector2 bottomLeft  = { (float)(cx - half), (float)(cy + half) };
    Vector2 bottomRight = { (float)(cx + half), (float)(cy + half) };

    Vector2 p1, p2, p3;
    float labelX, labelY;

    switch (dir) {
        case 0: // north
            p1 = topLeft; p2 = center; p3 = topRight;
            labelX = cx; labelY = cy - half * 0.62f;
            break;
        case 1: // south
            p1 = bottomLeft; p2 = bottomRight; p3 = center;
            labelX = cx; labelY = cy + half * 0.38f;
            break;
        case 2: // west
            p1 = topLeft; p2 = bottomLeft; p3 = center;
            labelX = cx - half * 0.62f; labelY = cy;
            break;
        case 3: // east
        default:
            p1 = topRight; p2 = center; p3 = bottomRight;
            labelX = cx + half * 0.38f; labelY = cy;
        break;
}

    DrawTriangle(p1, p2, p3, col);
    DrawTriangleLines(p1, p2, p3, { 30, 30, 30, 255 });

    string label = fmtQ(qVal);
    Vector2 textSize = MeasureTextEx(font, label.c_str(), fontSize, 1);
    DrawTextEx(font, label.c_str(),
               { labelX - textSize.x / 2, labelY - textSize.y / 2 },
               fontSize, 1, WHITE);
}

// draws a single grid cell at pixel position (px, py) with size cellSize
static void drawCell(int px, int py, int cellSize, int row, int col,
                     QTable& qt, World& w, bool holding,
                     double minVal, double maxVal,
                     Font font, int fontSize) {
    int cx = px + cellSize / 2;
    int cy = py + cellSize / 2;
    int half = cellSize / 2 - 2;

    // determine s, t, u based on world grid state
    auto grid = w.getGrid();
    bool s_val, t_val, u_val;
    if (holding) {
        s_val = grid[1][4] < 0;
        t_val = grid[4][0] < 0;
        u_val = grid[4][2] < 0;
    } else {
        s_val = grid[0][0] > 0;
        t_val = grid[2][2] > 0;
        u_val = grid[4][4] > 0;
    }

    int stateCode = encodeState(row, col, holding, s_val, t_val, u_val);

    // draw background
    DrawRectangle(px, py, cellSize, cellSize, { 20, 20, 20, 255 });

    // check if this is a pickup or dropoff cell and show a special overlay
    int gridVal = grid[row][col];
    bool isPickup  = (gridVal > 0);
    bool isDropoff = (gridVal < 0);

    // draw 4 directional triangles (n, s, w, e)
    // only draw directions that are valid (not out of bounds)
    bool canN = (row > 0);
    bool canS = (row < 4);
    bool canW = (col > 0);
    bool canE = (col < 4);

    if (canN) drawDirectionTriangle(cx, cy, half, 0, qt.get(stateCode, 'n'), minVal, maxVal, font, fontSize);
    if (canS) drawDirectionTriangle(cx, cy, half, 1, qt.get(stateCode, 's'), minVal, maxVal, font, fontSize);
    if (canW) drawDirectionTriangle(cx, cy, half, 2, qt.get(stateCode, 'w'), minVal, maxVal, font, fontSize);
    if (canE) drawDirectionTriangle(cx, cy, half, 3, qt.get(stateCode, 'e'), minVal, maxVal, font, fontSize);

    // if on an edge, fill unused triangle space with dark color
    if (!canN) DrawTriangle({(float)(cx-half),(float)(cy-half)}, {(float)cx,(float)cy}, {(float)(cx+half),(float)(cy-half)}, {15,15,15,255});
    if (!canS) DrawTriangle({(float)(cx-half),(float)(cy+half)}, {(float)(cx+half),(float)(cy+half)}, {(float)cx,(float)cy}, {15,15,15,255});
    if (!canW) DrawTriangle({(float)(cx-half),(float)(cy-half)}, {(float)(cx-half),(float)(cy+half)}, {(float)cx,(float)cy}, {15,15,15,255});
    if (!canE) DrawTriangle({(float)(cx+half),(float)(cy-half)}, {(float)cx,(float)cy}, {(float)(cx+half),(float)(cy+half)}, {15,15,15,255});

    // cell border
    DrawRectangleLines(px, py, cellSize, cellSize, { 60, 60, 60, 255 });

    // pickup/dropoff badge
    if (isPickup || isDropoff) {
    Color badgeColor;
    if (isPickup) {
        badgeColor = {0, 180, 80, 200};
    } else {
        badgeColor = {180, 40, 40, 200};
    }
    string badge = isPickup ? "P" : "D";
    DrawRectangle(px + cellSize - 18, py + 2, 16, 16, badgeColor);
    Vector2 bSize = MeasureTextEx(font, badge.c_str(), 11, 1);
    DrawTextEx(font, badge.c_str(),
               {(float)(px + cellSize - 18) + (16 - bSize.x) / 2,
                (float)(py + 2) + (16 - bSize.y) / 2},
               11, 1, WHITE);
}

    // coordinate label (small, bottom-left)
    string coord = "(" + to_string(row) + "," + to_string(col) + ")";
    DrawTextEx(font, coord.c_str(), {(float)(px + 3), (float)(py + cellSize - 13)}, 10, 1, {120, 120, 120, 255});
}

// finds the global min/max Q-values across motion actions only (n, s, w, e)
static pair<double,double> getQRange(QTable& qt, World& w) {
    vector<double> vals;
    auto& data = qt.data();
    for (auto it = data.begin(); it != data.end(); ++it) {
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            if (jt->first == 'p' || jt->first == 'd') continue;
            vals.push_back(jt->second);
        }
    }
    if (vals.empty()) return make_pair(0.0, 0.0);

    sort(vals.begin(), vals.end());

    int lo_idx = (int)(vals.size() * 0.05);
    int hi_idx = (int)(vals.size() * 0.95);
    // clamp indices to valid range
    lo_idx = max(0, min(lo_idx, (int)vals.size() - 1));
    hi_idx = max(0, min(hi_idx, (int)vals.size() - 1));

    return make_pair(vals[lo_idx], vals[hi_idx]);
}

// main visualizer function that displasy QTable
static void visualizeQTable(QTable& qt, World& w, bool holding, const string& title) {
    const int CELL   = 110;
    const int COLS   = 5;
    const int ROWS   = 5;
    const int MARGIN = 30;
    const int FOOTER = 50;
    const int WIDTH  = COLS * CELL + MARGIN * 2;
    const int HEIGHT = ROWS * CELL + MARGIN * 2 + FOOTER;

    InitWindow(WIDTH, HEIGHT, ("Q-Table Visualizer: " + title).c_str());
    SetTargetFPS(60);

    Font font = GetFontDefault();
    int fontSize = 11;

    auto [lo, hi] = getQRange(qt, w);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({10, 10, 10, 255});

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                int px = MARGIN + col * CELL;
                int py = MARGIN + row * CELL;
                drawCell(px, py, CELL, row, col, qt, w, holding, lo, hi, font, fontSize);
            }
        }

        // footer title
        string modeLabel = holding ? "[HOLDING BLOCK]" : "[NOT HOLDING]";
        string fullTitle = title + "  " + modeLabel;
        Vector2 tSize = MeasureTextEx(font, fullTitle.c_str(), 15, 1);
        DrawTextEx(font, fullTitle.c_str(),
                   {(float)(WIDTH / 2) - tSize.x / 2, (float)(ROWS * CELL + MARGIN + 12)},
                   15, 1, {200, 200, 200, 255});

        // range legend
        string rangeStr = "Range: [" + fmtQ(lo) + ", " + fmtQ(hi) + "]";
        DrawTextEx(font, rangeStr.c_str(), {(float)MARGIN, (float)(ROWS * CELL + MARGIN + 30)}, 11, 1, {130, 130, 130, 255});

        // press h hint
        string hint = "Press H to toggle holding / ESC to close";
        Vector2 hSize = MeasureTextEx(font, hint.c_str(), 10, 1);
        DrawTextEx(font, hint.c_str(),
                   {(float)(WIDTH) - hSize.x - MARGIN, (float)(ROWS * CELL + MARGIN + 33)},
                   10, 1, {90, 90, 90, 255});

        EndDrawing();

        // toggle holding state with H key
        if (IsKeyPressed(KEY_H)) {
            holding = !holding;
        }
    }

    CloseWindow();
}

#endif
