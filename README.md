# Unreal Chess

언리얼 연습용 체스 구현 과제


## Preview
<p align="center">
<img src="https://user-images.githubusercontent.com/46084498/145543787-e7f5249f-518a-404c-a16e-4c8e7057ffc3.gif" />
</p>

## Reference
Clubhouse Games: 51 Worldwide Classics

<p align="center">
<img src="https://user-images.githubusercontent.com/101321313/176491675-2b6a57f3-add4-4196-9ddd-4d1e63ad825a.png" width="70%" height="70%"/>
</p>

위 게임을 참고하여 최대한 비슷하게 구현하기 위해 노력했다. [Threat Map](#threat-map)이나, 그 외에 현재 선택한 Piece의 위치와 가능한 움직임들을 네모박스로 보여주는 기능, 현재 Piece를 선택하면 해당 Piece를 들어올리고 원래 위치는 반투명하게 보이게 하는 기능, 왼쪽 위에 현재 선택한 기물을 보여주는 기능 등등 대부분의 방법들 모두 해당 게임을 참고했다. 


## 개발 내용
- 컨벤션은 [언리얼 코딩 표준](https://docs.unrealengine.com/4.27/ko/ProductionPipelines/DevelopmentSetup/CodingStandard/)을 참고했다.
### Piece
- 기본 Piece 클래스를 구현하고, 각 기물들마다 Piece 클래스를 상속받아 각자의 기본 행마법을 구현했다. (BieshopPiece, KingPiece 등등)
- 그 중에서도 KingPiece의 경우, Checkmate를 확인하기 위해 Enemy에게 위협받고 있는 지를 구하는 함수를 추가로 구현했다.
- 특별 행마법의 일부는 구현하지 못했다.
  - 예를 들어, 폰의 경우 전진만 하지만 적을 잡을 때는 대각선 전방에 있는 적을 잡을 수 있다. 이건 구현했지만, 폰의 앙파상이나 프로모션같은 행마법까지는 구현하지 못했다.
- 각 Piece의 기본 움직임을 구할 때, 해당 움직임으로 인하여 King이 Checkmate가 된다면 해당 움직임은 불가능한 움직임이다. 따라서 해당 움직임들은 나중에 제거해줬다.
- 마찬가지로 Check상태일 때는 Check를 벗어나는 움직임만 할 수 있다.

### Threat Map
- 버튼을 클리하여 ThreatMap 기능을 킨다면 ThreatMap을 볼 수 있다.
- 각 칸마다 현재 플레이어와 적 플레이어의 Piece가 얼마나 올 수 있는지 판단하여, 현재 플레이어의 색은 파란색, 적 플레이어의 색은 붉은색 계열로 표시한다.
- 1개 이상 도달할 수 있다면 색이 진해지고, 현재 플레이어와 적 플레이어가 모두 도달할 수 있다면 그 중 더 많이 도달할 수 있는 쪽이 영역을 더 차지해서 표시된다.

### 게임 진행 상태?
- 게임의 상태를 배열로 저장해서 현재 Piece들이 어디 있는 지 저장해둘 수도 있겠지만, Unreal 연습용 과제인만큼 Trace(Raycast, 충돌)를 이용해보았다.
- 공간 상의 어떤 위치에 Piece가 있는 지 알기 위해서, 언리얼의 LineTraceSingleByObjectType을 이용하여 시작~끝 위치에 선을 그리고 충돌하는 액터(객체), Piece가 있는 지 찾는다.
  - 각 Piece들의 가능한 움직임을 구할 때, 기본 행마법과 충돌을 이용하여 각 움직임이 가능한 움직임(현재 플레이어의 Piece로 막혀 있지 않는 움직임이나, 폰이라면 적 Piece가 없다거나)을 구한다.
  - 또는 현재 플레이어가 들고 있던 Piece를 보드에 둘 때에도, 두는 위치에 적 Piece가 있는 지도 충돌을 이용해 확인했다.

### 게임 종료
- Checkmate거나 Stalemate일 때 게임이 종료된다.



