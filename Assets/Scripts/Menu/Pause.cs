using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pause : MonoBehaviour
{
    public GameObject pausePanel;

    public void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            TapPause();
        }
    }
    public void TapPause()
    {
        pausePanel.SetActive(!pausePanel.activeSelf);
        Time.timeScale = pausePanel.activeSelf ? 0f : 1f;
    }
}
