using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class DescriptionMaker 
{
   public static string GetDescription(Cell cell, CellInfo cellInfo)
    {
        if (cell == null)
        {
            return "---------";
        }
        string result = "";
        result = cellInfo.baseDiscription;

        /*if (cell.id == 3)
        {
            result += "\n Размер деревни: " + ((Village)cell).type.ToString();
        }*/

        if (cell.id == 5 && ((Town)cell).isSieged)
        {
            result += "\nГород сейчас осаждается.";
        }

        if (cell.daysBeforeUpdate > 0)
        {
            result += "\n(Недавно исследовано.\nОбновится через " + cell.daysBeforeUpdate.ToString() + " дней)";
        }
        return result;
    }
}
