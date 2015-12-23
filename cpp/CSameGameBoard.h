class CSameGameBoard
{
public:
  CSameGameBoard(void);
  ~CSameGameBoard(void);
  void SetupBoard(void);
  COLORREF GetBoardSpace(int row, int col);
  int GetWidth(void) const { return m_nWidth; }
  int GetHeight(void) const { return m_nHeight; }
  int GetColumns(void) const { return m_nColumns; }
  int GetRows(void) const { return m_nRows; }
  void DeleteBoard(void);
private:
  void CreateBoard(void);
  int** m_arrBoard; # 2D array pointer
  COLORREF m_arrColors[4]; # List of colors, 0 is background and 1-3 are piece colors
  int m_nColumns;
  int m_nRows;
  int m_nHeight;
  int m_nWidth;
};