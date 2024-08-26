using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class MercenarySquadPanel : MonoBehaviour
{
    public TextMeshProUGUI squadPosition;
    private WorldGenerator generator;

    public GameObject explainButton;

    private void Start()
    {
        generator = GameObject.Find("generator").GetComponent<WorldGenerator>();
    }

    public void UpdateText(Cell cell, List<Vector2Int> path = null)
    {
        if(path != null && path.Count > 0)
        {
            float time = generator.GetDaysToPath(path);
            squadPosition.text = "На пути в " + generator.GetCellInfo(cell).title + ".\n Будем на месте через " + time.ToString("0") + " дней.";
            explainButton.SetActive(false);
        }
        else
        {
            squadPosition.text = generator.GetCellInfo(cell).title + ".\n " + DescriptionMaker.GetDescription(cell, generator.GetCellInfo(cell));
            explainButton.SetActive(true);
        }
    }
}
