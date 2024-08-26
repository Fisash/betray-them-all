using UnityEngine;
using TMPro;

public class Hint : MonoBehaviour
{
    private TextMeshProUGUI nameText;
    private TextMeshProUGUI descriptionText;
    private GameObject tooltipPanel;

    private RectTransform tooltipRectTransform;

    private Marker marker;

    void Start()
    {
        marker = GameObject.Find("marker").GetComponent<Marker>();
        tooltipPanel = transform.GetChild(0).gameObject;
        tooltipRectTransform = tooltipPanel.GetComponent<RectTransform>();
        nameText = tooltipPanel.transform.GetChild(0).gameObject.GetComponent<TextMeshProUGUI>();
        descriptionText = tooltipPanel.transform.GetChild(1).gameObject.GetComponent<TextMeshProUGUI>();

        tooltipPanel.SetActive(false);
    }

    void Update()
    {
        if (Input.GetMouseButton(2))
        {
            Cell markedCell = marker.GetMarkedCell();
            if (markedCell == null)
            {
                tooltipPanel.SetActive(false);
                return;
            }
            CellInfo markedCellInfo = marker.GetMarkedCellInfo();
            nameText.text = markedCellInfo.title;
            descriptionText.text = DescriptionMaker.GetDescription(markedCell, markedCellInfo);
            Vector3 mousePosition = Input.mousePosition;
            Vector3 worldPosition = Camera.main.ScreenToWorldPoint(mousePosition);
            worldPosition.x = Mathf.Round(worldPosition.x);
            worldPosition.y = Mathf.Round(worldPosition.y);
            Vector3 localPosition = tooltipRectTransform.parent.InverseTransformPoint(worldPosition);
            localPosition.y += 180f;


            tooltipRectTransform.localPosition = new Vector3(localPosition.x, localPosition.y, 0);
            tooltipPanel.SetActive(true);
        }
        else
        {
            tooltipPanel.SetActive(false); 
        }
    }
}